"""Gera as figuras dos 2 grupos cross-class confirmados."""
from pathlib import Path
import pandas as pd
from curadoria.visualize import plot_cross_class_group


DATASET = Path("Brain Tumor MRI Dataset")
INV = pd.read_csv("resultados/inventario.csv", encoding="utf-8-sig")

GROUPS = {
    "grupo_A_te_gl_68": [
        "Testing/glioma/Te-gl_68.jpg",
        "Testing/meningioma/Te-me_50.jpg",
        "Testing/meningioma/Te-me_61.jpg",
        "Testing/meningioma/Te-me_96.jpg",
        "Training/meningioma/Tr-me_458.jpg",
        "Training/meningioma/Tr-me_468.jpg",
        "Training/meningioma/Tr-me_714.jpg",
    ],
    "grupo_B_te_gl_160": [
        "Testing/glioma/Te-gl_160.jpg",
        "Testing/meningioma/Te-me_159.jpg",
        "Training/meningioma/Tr-me_1122.jpg",
    ],
}

meta = INV.set_index("relative_path")[["split", "class_label"]]

for name, paths in GROUPS.items():
    members = [
        {
            "relative_path": p,
            "class": meta.loc[p, "class_label"],
            "split": meta.loc[p, "split"],
        }
        for p in paths
    ]
    plot_cross_class_group(
        DATASET,
        members,
        output_png=Path("figuras") / f"{name}.png",
        title=f"{name} — SSIM ≥ 0,96",
    )