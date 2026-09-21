"""Pontuação fina de pares candidatos (camada 3) por MAE e SSIM estrutural.

Este módulo NUNCA decide se um par é duplicata. Ele só calcula métricas e
ordena os pares por prioridade de revisão humana, seguindo o pré-projeto:

    "Pares serão registrados, agrupados e inspecionados por amostragem,
    sem exclusão automática de imagens semelhantes, pois podem representar
    cortes distintos de um mesmo exame."

Por isso não existe aqui nenhuma coluna "confirmed"/"confirmado": existe
"sugestao_revisao", que é só uma referência textual para orientar o
inspetor humano, igual à função já usada na ferramenta de revisão manual.
A decisão de fato (mantida ou excluída) só é registrada pelo módulo
`decisao.py`, e só a partir de uma avaliação humana salva por `revisao.py`.
"""
from __future__ import annotations

from pathlib import Path

import numpy as np
import pandas as pd
from PIL import Image
from skimage.metrics import structural_similarity
from tqdm import tqdm

COMPARISON_SIZE = (225, 225)

# Limiares provisórios para a sugestão textual — NÃO validados estatisticamente
# nesta base ainda. Devem ser recalibrados quando houver revisão manual
# suficiente (ver migracao/dados_legados, que já traz 221 pares rotulados
# manualmente como ponto de partida para essa calibração).
MAE_BAIXO = 15.0
MAE_ALTO = 30.0
SSIM_ALTO = 0.90
SSIM_BAIXO = 0.75
LIMIAR_SINAL = 0.15


def _load_gray(path: Path, size: tuple[int, int] = COMPARISON_SIZE) -> np.ndarray:
    with Image.open(path) as img:
        return np.array(
            img.convert("L").resize(size, Image.Resampling.LANCZOS),
            dtype=np.uint8,
        )


def _best_alignment(a: np.ndarray, b: np.ndarray) -> tuple[float, float, int]:
    """Testa as 4 rotações de B contra A; escolhe a de menor MAE.

    Retorna (mae, ssim, angulo_graus) na rotação escolhida pelo MAE.
    O SSIM é calculado na MESMA rotação escolhida pelo MAE (não busca o
    melhor SSIM independentemente), para manter os dois valores comparáveis
    par a par — os critérios podem discordar, e é justamente essa
    discordância (o "sinal") que ajuda a apontar diluição de fundo preto.
    """
    best_mae = float("inf")
    best_rot = 0
    for k in range(4):
        rb = np.ascontiguousarray(np.rot90(b, k))
        mae = float(np.abs(a.astype(np.int16) - rb.astype(np.int16)).mean())
        if mae < best_mae:
            best_mae = mae
            best_rot = k * 90

    rb = np.ascontiguousarray(np.rot90(b, best_rot // 90))
    ssim = float(structural_similarity(a, rb, data_range=255, channel_axis=None))
    return best_mae, ssim, best_rot


def sugerir_relacao(mae: float, ssim: float) -> str:
    """Referência textual para o inspetor humano; nunca decide sozinha."""
    mae_norm = 1.0 - (mae / 255.0)
    sinal = ssim - mae_norm

    if mae < MAE_BAIXO and ssim > SSIM_ALTO:
        return "provavel_duplicata (pixels e estrutura concordam)"
    if mae > MAE_ALTO and ssim < SSIM_BAIXO:
        return "possiveis_imagens_diferentes (pixels e estrutura concordam)"
    if sinal > LIMIAR_SINAL:
        return "possivel_duplicata_com_brilho_contraste_diferente"
    if sinal < -LIMIAR_SINAL:
        return "possiveis_imagens_diferentes (cuidado com diluicao do fundo no MAE)"
    return "inconclusivo_avaliar_visualmente"


def score_pairs(dataset_dir: Path, pairs_path: Path, output_path: Path) -> pd.DataFrame:
    """Calcula MAE, SSIM e uma sugestão textual para cada par candidato.

    Não filtra, não decide e não escreve nenhuma coluna de confirmação.
    A ordenação de saída (por SSIM decrescente) é só uma priorização de
    fila de revisão, não uma classificação de verdade.
    """
    pairs = pd.read_csv(pairs_path, encoding="utf-8-sig")
    if pairs.empty:
        print("Nenhum par candidato para pontuar.")
        for col in ("mean_absolute_difference", "ssim", "best_rotation_b", "sugestao_revisao"):
            pairs[col] = []
        pairs.to_csv(output_path, index=False, encoding="utf-8-sig")
        return pairs

    print(f"Pares a pontuar: {len(pairs)}")

    cache: dict[str, np.ndarray] = {}

    def get(rel: str) -> np.ndarray:
        if rel not in cache:
            cache[rel] = _load_gray(dataset_dir / rel)
        return cache[rel]

    records = []
    for row in tqdm(pairs.to_dict("records"), unit="par", desc="MAE+SSIM"):
        a = get(row["relative_path_a"])
        b = get(row["relative_path_b"])
        mae, ssim, rot = _best_alignment(a, b)
        records.append({
            **row,
            "mean_absolute_difference": round(mae, 4),
            "ssim": round(ssim, 4),
            "best_rotation_b": rot,
            "sugestao_revisao": sugerir_relacao(mae, ssim),
        })

    df = pd.DataFrame(records).sort_values("ssim", ascending=False)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    df.to_csv(output_path, index=False, encoding="utf-8-sig")

    print("\nRESULTADO DA PONTUAÇÃO (não é decisão — apenas ordena a fila de revisão)")
    print(f"  Pares pontuados: {len(df)}")
    for rotulo in df["sugestao_revisao"].unique():
        print(f"  {rotulo}: {int((df['sugestao_revisao'] == rotulo).sum())}")
    return df
