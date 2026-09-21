"""Registro de decisões finais de curadoria — sempre por ato humano explícito.

Este é o ÚNICO módulo que pode marcar uma imagem como excluída da base
curada. Ele nunca deriva isso sozinho a partir de SSIM, de
`visual_assessment` ou de qualquer outra métrica: cada linha do arquivo
de decisões precisa ter sido escrita por uma pessoa, com um motivo.

Fluxo pretendido:
1. `resumo_para_decisao` lê o CSV de revisões (de `revisao.py`) e monta um
   relatório por cluster com pares avaliados como prováveis duplicatas —
   isso é só um RELATÓRIO para apoiar a decisão, não uma decisão.
2. A pessoa lê esse relatório, decide o que fazer, e registra a decisão
   com `registrar_decisao` (uma imagem por vez) ou preenchendo à mão um
   CSV de decisões e aplicando com `aplicar_decisoes_em_lote` — mas o CSV
   de entrada é sempre escrito por humano, nunca gerado automaticamente
   com "decision" e "reason" já preenchidos.
"""
from __future__ import annotations

from pathlib import Path

import pandas as pd

DECISOES_VALIDAS = {"mantida", "excluida"}


def resumo_para_decisao(revisoes: pd.DataFrame) -> pd.DataFrame:
    """Relatório por cluster para apoiar a decisão humana — não decide nada.

    Reúne, por cluster_id, os pares cuja avaliação humana (visual_assessment)
    apontou para duplicata, com as imagens envolvidas e os sinalizadores de
    risco (cross_split, cross_class quando presentes na revisão).
    """
    duplicata_like = revisoes[
        revisoes["visual_assessment"].isin(
            ["provavel_duplicata_visual", "provavel_duplicata_visual_com_rotacao"]
        )
    ]
    if duplicata_like.empty:
        return pd.DataFrame(columns=[
            "cluster_id", "relative_path_a", "relative_path_b",
            "visual_assessment", "observation", "ssim", "mean_absolute_difference",
        ])
    return duplicata_like[
        [
            "cluster_id", "relative_path_a", "relative_path_b",
            "visual_assessment", "observation", "ssim", "mean_absolute_difference",
        ]
    ].sort_values(["cluster_id", "ssim"], ascending=[True, False])


def registrar_decisao(
    controle_path: Path,
    relative_path: str,
    decision: str,
    reason: str,
) -> None:
    """Registra a decisão de UMA imagem no controle mestre de curadoria.

    `decision` precisa ser "mantida" ou "excluida"; `reason` é obrigatório
    e de texto livre (a pessoa escreve o motivo, não é gerado aqui).
    """
    if decision not in DECISOES_VALIDAS:
        raise ValueError(f"decision precisa ser um de {DECISOES_VALIDAS}, recebido: {decision!r}")
    if not reason or not reason.strip():
        raise ValueError("reason é obrigatório — descreva por que essa decisão foi tomada.")

    # dtype=str + keep_default_na=False evitam que colunas de texto vazias
    # (ex.: curation_reason ainda não preenchido) virem float64/NaN na
    # leitura, o que quebraria a escrita de um motivo em texto livre depois.
    df = pd.read_csv(controle_path, encoding="utf-8-sig", dtype=str, keep_default_na=False)
    mask = df["relative_path"] == relative_path
    if not mask.any():
        raise ValueError(f"relative_path não encontrado no controle: {relative_path}")

    df.loc[mask, "curation_status"] = decision
    df.loc[mask, "curation_reason"] = reason.strip()
    df.loc[mask, "decided_at"] = pd.Timestamp.now().isoformat()
    df.to_csv(controle_path, index=False, encoding="utf-8-sig")
    print(f"Registrado: {relative_path} -> {decision} ({reason.strip()})")


def aplicar_decisoes_em_lote(controle_path: Path, decisoes_manuais_path: Path) -> None:
    """Aplica um CSV de decisões escrito à mão (relative_path,decision,reason).

    Cada linha desse CSV de entrada precisa ter sido preenchida por uma
    pessoa. Este comando só aplica; não gera decisão nenhuma.
    """
    decisoes = pd.read_csv(decisoes_manuais_path, encoding="utf-8-sig")
    faltando = {"relative_path", "decision", "reason"} - set(decisoes.columns)
    if faltando:
        raise ValueError(f"CSV de decisões precisa das colunas {faltando}")

    for _, row in decisoes.iterrows():
        registrar_decisao(controle_path, row["relative_path"], row["decision"], row["reason"])
