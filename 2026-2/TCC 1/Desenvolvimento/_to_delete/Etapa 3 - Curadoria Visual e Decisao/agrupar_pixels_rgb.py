"""
Agrupa imagens de um componente por igualdade dos pixels RGB.

Não redimensiona imagens, não altera os arquivos originais e não toma
decisões de aprovação ou exclusão.
"""

import hashlib
from pathlib import Path

import numpy as np
import pandas as pd
from PIL import Image


# --- Configuração ------------------------------------------------------------

STAGE_DIR = Path(__file__).resolve().parent
DEVELOPMENT_DIR = STAGE_DIR.parent
DATASET_DIR = DEVELOPMENT_DIR.parent / "Brain Tumor MRI Dataset"

MEMBERSHIP_PATH = (
    DEVELOPMENT_DIR
    / "Etapa 2 - Deduplicacao e Sensibilidade"
    / "resultados"
    / "imagens_por_componente_t0.csv"
)

COMPONENT_ID = "t0_0134"
OUTPUT_DIR = STAGE_DIR / "resultados"


def load_rgb(relative_path: str) -> np.ndarray:
    """Lê os pixels RGB sem redimensionar ou alterar o original."""
    with Image.open(DATASET_DIR / relative_path) as image:
        return np.array(image.convert("RGB"))


def run() -> None:
    membership_df = pd.read_csv(
        MEMBERSHIP_PATH,
        encoding="utf-8-sig",
        dtype={
            "sha256_hash": "string",
            "phash": "string",
            "dhash": "string",
        },
    )

    component_df = (
        membership_df.loc[
            membership_df["component_id"] == COMPONENT_ID
        ]
        .sort_values("relative_path")
        .reset_index(drop=True)
    )

    if component_df.empty:
        raise ValueError(f"Componente não encontrado: {COMPONENT_ID}")

    if (
        component_df["relative_path"].isna().any()
        or component_df["relative_path"].duplicated().any()
    ):
        raise ValueError("Existem caminhos ausentes ou repetidos.")

    print(f"Componente: {COMPONENT_ID}")
    print(f"Imagens selecionadas: {len(component_df)}")
    print("Critério: mesmas dimensões e mesmos pixels após conversão RGB.")

    # Cada chave inclui as dimensões e o SHA-256 dos pixels RGB.
    candidate_groups = {}
    records = []

    for index, row in component_df.iterrows():
        relative_path = row["relative_path"]
        pixels = load_rgb(relative_path)

        height, width, channels = pixels.shape
        pixel_hash = hashlib.sha256(pixels.tobytes()).hexdigest()
        key = (height, width, channels, pixel_hash)

        if key in candidate_groups:
            group_info = candidate_groups[key]

            # Confirma a igualdade diretamente, sem depender só do hash.
            representative_pixels = load_rgb(
                group_info["representative_path"]
            )

            if not np.array_equal(pixels, representative_pixels):
                raise ValueError(
                    "Hash de pixels coincidente, mas arrays diferentes. "
                    "Análise interrompida para investigação."
                )
        else:
            group_info = {
                "group_id": (
                    f"{COMPONENT_ID}_rgb_{len(candidate_groups) + 1:03d}"
                ),
                "representative_path": relative_path,
            }
            candidate_groups[key] = group_info

        record = row.to_dict()
        record.update({
            "rgb_group_id": group_info["group_id"],
            "rgb_width": width,
            "rgb_height": height,
            "rgb_pixel_sha256": pixel_hash,
            # Representante técnico para comparação, não imagem aprovada.
            "comparison_representative": group_info["representative_path"],
        })
        records.append(record)

        print(f"Processadas: {index + 1}/{len(component_df)}")

    details_df = pd.DataFrame(records)

    summary_df = (
        details_df.groupby("rgb_group_id", sort=False)
        .agg(
            image_count=("relative_path", "size"),
            split_count=("split_assigned", "nunique"),
            class_count=("class_label", "nunique"),
            rgb_width=("rgb_width", "first"),
            rgb_height=("rgb_height", "first"),
            comparison_representative=("comparison_representative", "first"),
        )
        .reset_index()
    )

    summary_df["repeated_rgb"] = summary_df["image_count"] > 1
    summary_df["cross_split"] = summary_df["split_count"] > 1
    summary_df["different_class"] = summary_df["class_count"] > 1

    details_df = details_df.merge(
        summary_df[["rgb_group_id", "image_count"]],
        on="rgb_group_id",
        how="left",
        validate="many_to_one",
    )

    repeated_df = summary_df.loc[summary_df["repeated_rgb"]]
    repeated_images = int(repeated_df["image_count"].sum())
    excess_copies = int(
        (repeated_df["image_count"] - 1).sum()
    )

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    details_path = OUTPUT_DIR / f"{COMPONENT_ID}_imagens_por_grupo_rgb.csv"
    summary_path = OUTPUT_DIR / f"{COMPONENT_ID}_resumo_grupos_rgb.csv"

    # Relatórios analíticos regenerados a cada execução.
    # Os arquivos de decisões de curadoria não são modificados.
    details_df.to_csv(details_path, index=False, encoding="utf-8-sig")
    summary_df.to_csv(summary_path, index=False, encoding="utf-8-sig")

    print("\nRESULTADO DO AGRUPAMENTO RGB")
    print(f"Imagens analisadas: {len(details_df)}")
    print(f"Conteúdos RGB distintos: {len(summary_df)}")
    print(f"Grupos com pixels repetidos: {len(repeated_df)}")
    print(f"Imagens nesses grupos: {repeated_images}")
    print(f"Cópias excedentes teóricas: {excess_copies}")
    print(
        "Grupos RGB repetidos entre treino e teste:",
        int(repeated_df["cross_split"].sum()),
    )
    print(
        "Grupos RGB repetidos com classes diferentes:",
        int(repeated_df["different_class"].sum()),
    )

    for _, group in repeated_df.iterrows():
        members = details_df.loc[
            details_df["rgb_group_id"] == group["rgb_group_id"]
        ]

        print(
            f"\n{group['rgb_group_id']} — "
            f"{group['image_count']} imagens com pixels RGB iguais:"
        )
        for relative_path in members["relative_path"]:
            print(f"  {relative_path}")

    print(f"\nDetalhamento: {details_path}")
    print(f"Resumo: {summary_path}")
    print("\nRepresentantes são referências de comparação, não aprovações.")
    print("Nenhuma imagem ou decisão de curadoria foi alterada.")


if __name__ == "__main__":
    run()