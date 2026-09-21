from pathlib import Path

import numpy as np
import pandas as pd
from PIL import Image
from itertools import combinations

stage_dir = Path(__file__).resolve().parent
dataset_dir = stage_dir.parent.parent / "Brain Tumor MRI Dataset"

summary_path = (
    stage_dir
    / "resultados"
    / "t0_0134_resumo_grupos_rgb.csv"
)

summary_df = pd.read_csv(summary_path, encoding="utf-8-sig")

# Um representante por grupo de pixels RGB iguais.
representative_paths = (
    summary_df["comparison_representative"].tolist()
)

comparison_images = {}

for relative_path in representative_paths:
    with Image.open(dataset_dir / relative_path) as image:
        # Padronização somente para comparação exploratória.
        comparison_image = image.convert("RGB").resize(
            (225, 225),
            resample=Image.Resampling.LANCZOS,
        )

        comparison_images[relative_path] = np.array(
            comparison_image,
            dtype=np.int16,
        )

print(f"Representantes carregados: {len(comparison_images)}")
print("Dimensões para comparação: 225 × 225 pixels, RGB")
print("Nenhuma imagem original foi alterada.")

comparison_records = []

for path_a, path_b in combinations(representative_paths, 2):
    pixels_a = comparison_images[path_a]
    pixels_b = comparison_images[path_b]

    rotation_errors = []

    for quarter_turns in range(4):
        # np.rot90 gira no sentido anti-horário, somente em memória.
        rotated_b = np.rot90(pixels_b, k=quarter_turns)

        mean_difference = float(
            np.abs(pixels_a - rotated_b).mean()
        )
        rotation_errors.append(mean_difference)

    # Em caso de empate, escolhe o primeiro ângulo na ordem 0, 90, 180, 270.
    best_index = int(np.argmin(rotation_errors))

    comparison_records.append({
        "relative_path_a": path_a,
        "relative_path_b": path_b,
        "mae_0": rotation_errors[0],
        "mae_90": rotation_errors[1],
        "mae_180": rotation_errors[2],
        "mae_270": rotation_errors[3],
        "best_rotation_b": best_index * 90,
        "lowest_mae": rotation_errors[best_index],
        "cross_split": path_a.split("/")[0] != path_b.split("/")[0],
    })

rotation_df = pd.DataFrame(comparison_records).sort_values(
    ["lowest_mae", "relative_path_a", "relative_path_b"]
)

output_path = stage_dir / "resultados" / "t0_0134_comparacoes_rotacoes.csv"

rotation_df.to_csv(
    output_path,
    index=False,
    encoding="utf-8-sig",
)

print(f"\nPares comparados: {len(rotation_df)}")
print("Rotações avaliadas por par: 0°, 90°, 180° e 270°")
print("\nDez pares entre splits com menor diferença média:")

print(
    rotation_df.loc[
        rotation_df["cross_split"],
        [
            "relative_path_a",
            "relative_path_b",
            "best_rotation_b",
            "lowest_mae",
        ],
    ].head(10).to_string(index=False)
)

print(f"\nComparações salvas em: {output_path}")
print("A ordenação prioriza inspeções; não confirma duplicação.")