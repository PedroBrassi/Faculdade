"""Visualização de grupos cross-class confirmados (para figura do TCC)."""
from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from PIL import Image


def _load(dataset: Path, rel: str, size=(224, 224)) -> np.ndarray:
    with Image.open(dataset / rel) as img:
        return np.array(img.convert("L").resize(size, Image.Resampling.LANCZOS))


def plot_cross_class_group(
    dataset_dir: Path,
    members: list[dict],
    output_png: Path,
    title: str,
) -> None:
    """members: lista de {'relative_path':..., 'class':..., 'split':...}"""
    n = len(members)
    cols = min(4, n)
    rows = (n + cols - 1) // cols

    fig, axes = plt.subplots(rows, cols, figsize=(4 * cols, 4 * rows), squeeze=False)
    fig.suptitle(title, fontsize=12)

    for i, m in enumerate(members):
        ax = axes[i // cols][i % cols]
        ax.imshow(_load(dataset_dir, m["relative_path"]), cmap="gray")
        ax.axis("off")
        ax.set_title(
            f"{m['class']} | {m['split']}\n{Path(m['relative_path']).name}",
            fontsize=9,
        )

    for j in range(n, rows * cols):
        axes[j // cols][j % cols].axis("off")

    fig.tight_layout(rect=(0, 0, 1, 0.95))
    output_png.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(output_png, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"Figura salva em: {output_png}")