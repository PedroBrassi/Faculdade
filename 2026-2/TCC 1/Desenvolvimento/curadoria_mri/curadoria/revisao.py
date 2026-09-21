"""Revisão manual interativa de pares candidatos, com botões (matplotlib).

Continuação direta da ferramenta que já existia no projeto (antes chamada
"interface_botoes.py"), com duas mudanças estruturais:

1. Em vez de gravar um CSV por par (um arquivo com hash no nome para cada
   avaliação, inclusive re-avaliações via "Voltar"), este módulo mantém
   UM único CSV de revisões, com upsert por par (relative_path_a,
   relative_path_b). Reduz dezenas de arquivos soltos a um só, sem perder
   histórico de quem avaliou o quê.

2. A fila de pares a revisar não é fixada a um componente específico:
   vem de `montar_fila`, que junta os candidatos pontuados (`score.py`)
   com os clusters (`cluster.py`) e pula pares já revisados — inclusive
   os 221 pares migrados do fluxo antigo (ver `migracao/`).

Nenhuma decisão de exclusão é tomada aqui. O campo "decision" desta
ferramenta é sempre "pendente"; só o módulo `decisao.py`, a partir de uma
ação humana explícita, pode mudar isso.
"""
from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.widgets import Button, TextBox
from PIL import Image

from .score import sugerir_relacao

COMPARISON_SIZE = (225, 225)

REVIEW_COLUMNS = [
    "cluster_id", "relative_path_a", "relative_path_b",
    "mean_absolute_difference", "ssim", "best_rotation_b",
    "sugestao_revisao", "visual_assessment", "observation",
    "decision", "reviewed_at",
]

CHOICES = [
    ("Provável duplicata", "provavel_duplicata_visual"),
    ("Provável com rotação", "provavel_duplicata_visual_com_rotacao"),
    ("Aparentemente distintas", "aparentemente_distintas"),
    ("Inconclusivo", "inconclusivo"),
]


def _pair_key(a: str, b: str) -> tuple[str, str]:
    """Chave estável e não-ordenada para o par (independe de quem é A/B)."""
    return (a, b) if a <= b else (b, a)


def carregar_revisoes(caminho: Path) -> pd.DataFrame:
    if caminho.exists():
        return pd.read_csv(caminho, encoding="utf-8-sig")
    return pd.DataFrame(columns=REVIEW_COLUMNS)


def montar_fila(scored_pairs: pd.DataFrame, revisoes_existentes: pd.DataFrame) -> list[dict]:
    """Pares pontuados que ainda não têm avaliação humana salva."""
    revisados = {
        _pair_key(r["relative_path_a"], r["relative_path_b"])
        for _, r in revisoes_existentes.iterrows()
    }
    fila = []
    for _, row in scored_pairs.iterrows():
        key = _pair_key(row["relative_path_a"], row["relative_path_b"])
        if key in revisados:
            continue
        fila.append(row.to_dict())
    return fila


def salvar_revisao(
    caminho: Path,
    par: dict,
    avaliacao: str,
    observacao: str,
) -> None:
    """Upsert da avaliação de UM par no CSV único de revisões."""
    df = carregar_revisoes(caminho)
    key = _pair_key(par["relative_path_a"], par["relative_path_b"])
    if not df.empty:
        existing_keys = df.apply(
            lambda r: _pair_key(r["relative_path_a"], r["relative_path_b"]), axis=1
        )
        df = df[existing_keys != key]

    novo = {
        "cluster_id": par.get("cluster_id", ""),
        "relative_path_a": par["relative_path_a"],
        "relative_path_b": par["relative_path_b"],
        "mean_absolute_difference": par.get("mean_absolute_difference", ""),
        "ssim": par.get("ssim", ""),
        "best_rotation_b": par.get("best_rotation_b", ""),
        "sugestao_revisao": par.get("sugestao_revisao", ""),
        "visual_assessment": avaliacao,
        "observation": observacao,
        "decision": "pendente",
        "reviewed_at": pd.Timestamp.now().isoformat(),
    }
    df = pd.concat([df, pd.DataFrame([novo])], ignore_index=True)
    caminho.parent.mkdir(parents=True, exist_ok=True)
    df.to_csv(caminho, index=False, encoding="utf-8-sig")


def _load_rgb(dataset_dir: Path, rel: str, size=COMPARISON_SIZE) -> np.ndarray:
    with Image.open(dataset_dir / rel) as img:
        return np.array(img.convert("RGB").resize(size, Image.Resampling.LANCZOS))


class JanelaRevisao:
    def __init__(self, fila: list[dict], dataset_dir: Path, caminho_saida: Path):
        self.fila = fila
        self.dataset_dir = dataset_dir
        self.caminho_saida = caminho_saida
        self.index = 0
        self.saved: dict[int, tuple[str, str]] = {}
        self.busy = False
        self.ready = False

        self.figure, self.axes = plt.subplots(1, 3, figsize=(15, 8))
        self.figure.subplots_adjust(left=.035, right=.97, bottom=.34, top=.80, wspace=.12)
        self.figure.canvas.manager.set_window_title("Curadoria — revisão manual")
        self.title = self.figure.suptitle("", fontsize=13, y=.97)
        self.status = self.figure.text(.04, .295, "", fontsize=10)
        self.note = TextBox(self.figure.add_axes([.14, .22, .82, .045]), "Observação: ")
        self.buttons: list[Button] = []
        for i, (label, value) in enumerate(CHOICES):
            self._add_button(
                [.035 + i * .24, .135, .225, .055], label,
                lambda event, assessment=value: self._assess(assessment),
            )
        self._add_button([.035, .055, .21, .055], "Voltar", self._back)
        self._add_button([.275, .055, .21, .055], "Pular / avançar", self._skip)
        self._add_button([.515, .055, .21, .055], "Encerrar", lambda e: plt.close(self.figure))
        self.figure.text(
            .04, .012,
            "Clicar em uma avaliação salva e avança. Decisão final continua pendente "
            "(ver decisao.py).",
            fontsize=9,
        )
        self._draw()

    def _add_button(self, position, label, callback) -> None:
        button = Button(self.figure.add_axes(position), label, color="#e9eef5", hovercolor="#d2e3f5")
        button.label.set_fontsize(9)
        button.on_clicked(callback)
        self.buttons.append(button)

    def _message(self, text: str, error: bool = False) -> None:
        self.status.set_text(text)
        self.status.set_color("firebrick" if error else "#214b36")
        self.figure.canvas.draw_idle()

    @staticmethod
    def _label(path: str) -> str:
        parts = path.replace("\\", "/").split("/")
        split = {"Training": "treino", "Testing": "teste"}.get(parts[0], parts[0])
        return f"{parts[-1]}\n{split} | classe: {parts[-2]}"

    def _draw(self) -> None:
        self.ready = False
        for axis in self.axes:
            axis.clear()
            axis.axis("off")

        if self.index >= len(self.fila):
            self.title.set_text("Fim da fila desta sessão")
            self.note.set_val("")
            self._message(f"{len(self.saved)} pares avaliados nesta sessão. Encerrar quando quiser.")
            return

        par = self.fila[self.index]
        angle = int(par.get("best_rotation_b", 0) or 0)
        self.title.set_text(f"Par {self.index + 1}/{len(self.fila)} — carregando...")

        try:
            a = _load_rgb(self.dataset_dir, par["relative_path_a"])
            b = _load_rgb(self.dataset_dir, par["relative_path_b"])

            mae = par.get("mean_absolute_difference")
            ssim = par.get("ssim")
            sugestao = par.get("sugestao_revisao") or sugerir_relacao(float(mae), float(ssim))

            self.title.set_text(
                f"Par {self.index + 1}/{len(self.fila)} | Avaliados nesta sessão: {len(self.saved)}\n"
                f"MAE: {float(mae):.2f} | SSIM: {float(ssim):.4f} | "
                f"cluster {par.get('cluster_id', '?')}\n"
                f"Sugestão: {sugestao}"
            )

            views = [
                (a, "A — " + self._label(par["relative_path_a"])),
                (b, "B — " + self._label(par["relative_path_b"])),
                (np.rot90(b, angle // 90), f"B — rotação de {angle}°\nSomente para comparação"),
            ]
            for axis, (pixels, label) in zip(self.axes, views):
                axis.imshow(pixels)
                axis.set_title(label, fontsize=10)

            self.ready = True
            saved = self.saved.get(self.index)
            self.note.set_val(saved[1] if saved else "")
            if saved:
                self._message(f"Avaliação salva: {saved[0]}. Outro clique corrige.")
            else:
                self._message("Escolha uma avaliação ou pule. Observação é opcional.")
        except Exception as error:  # noqa: BLE001 — mostra na UI e segue
            self.title.set_text(f"Par {self.index + 1}/{len(self.fila)} — falha")
            self._message(f"Falha ao carregar ou calcular métricas: {error}. Pode pular ou encerrar.", True)

    def _assess(self, assessment: str) -> None:
        if self.busy or not self.ready or self.index >= len(self.fila):
            return
        self.busy = True
        try:
            observation = self.note.text.strip()
            salvar_revisao(self.caminho_saida, self.fila[self.index], assessment, observation)
            self.saved[self.index] = (assessment, observation)
            self.index += 1
            self._draw()
        except Exception as error:  # noqa: BLE001
            self._message(f"Não foi salvo: {error}", True)
        finally:
            self.busy = False

    def _skip(self, event=None) -> None:
        if not self.busy and self.index < len(self.fila):
            self.index += 1
            self._draw()

    def _back(self, event=None) -> None:
        if not self.busy and self.index > 0:
            self.index -= 1
            self._draw()

    def mostrar(self) -> None:
        plt.show(block=True)
