from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from PIL import Image


# --- Configuração ------------------------------------------------------------

stage_dir = Path(__file__).resolve().parent
development_dir = stage_dir.parent
dataset_dir = development_dir.parent / "Brain Tumor MRI Dataset"

membership_path = (
    development_dir
    / "Etapa 2 - Deduplicacao e Sensibilidade"
    / "resultados"
    / "imagens_por_componente_t0.csv"
)

COMPONENT_ID = "t0_0134"

comparison_paths = [
    "Testing/notumor/Te-no_113.jpg",
    "Testing/notumor/Te-no_140.jpg",
]


# --- Leitura e conferência ----------------------------------------------------

membership_df = pd.read_csv(
    membership_path,
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

if component_df["relative_path"].duplicated().any():
    raise ValueError("O componente contém caminhos repetidos.")

if component_df["sha256_hash"].isna().any():
    raise ValueError("Existem imagens sem SHA-256 no componente.")

component_paths = set(component_df["relative_path"])

if not set(comparison_paths).issubset(component_paths):
    raise ValueError(
        "Uma das imagens selecionadas não pertence ao componente."
    )

print(f"Componente: {COMPONENT_ID}")
print(f"Imagens no componente: {len(component_df)}")


# --- Duplicatas exatas --------------------------------------------------------

exact_groups = (
    component_df.groupby("sha256_hash")["relative_path"]
    .agg(list)
)

repeated_groups = exact_groups.loc[
    exact_groups.map(len) > 1
]

print("\nDUPLICATAS EXATAS DENTRO DO COMPONENTE")
print(f"Grupos com SHA-256 repetido: {len(repeated_groups)}")
print(
    "Arquivos nesses grupos:",
    sum(len(paths) for paths in repeated_groups),
)
print(
    "Cópias excedentes teóricas:",
    sum(len(paths) - 1 for paths in repeated_groups),
)

for index, paths in enumerate(repeated_groups, start=1):
    print(f"\nGrupo exato {index} — {len(paths)} arquivos:")

    for relative_path in paths:
        print(f"  {relative_path}")

# Um representante por conteúdo exato, apenas para organizar a inspeção.
# A seleção não é uma decisão sobre qual arquivo manter na base curada.
representatives_df = (
    component_df
    .drop_duplicates(subset="sha256_hash", keep="first")
    .copy()
)

print(f"\nRepresentantes para inspeção: {len(representatives_df)}")


# --- Comparação do par selecionado -------------------------------------------

print("\nPAR EM COMPARAÇÃO")
print(f"A: {comparison_paths[0]}")
print(f"B: {comparison_paths[1]}")

metadata = component_df.set_index("relative_path")

same_sha256 = (
    metadata.loc[comparison_paths[0], "sha256_hash"]
    == metadata.loc[comparison_paths[1], "sha256_hash"]
)

print(f"SHA-256 iguais no manifesto: {same_sha256}")

# Converte apenas em memória, sem redimensionar ou modificar os originais.
with Image.open(dataset_dir / comparison_paths[0]) as image_a:
    pixels_a = np.array(image_a.convert("RGB"))

with Image.open(dataset_dir / comparison_paths[1]) as image_b:
    pixels_b = np.array(image_b.convert("RGB"))

print(f"Dimensões RGB de A: {pixels_a.shape}")
print(f"Dimensões RGB de B: {pixels_b.shape}")

same_shape = pixels_a.shape == pixels_b.shape
print(f"Mesmas dimensões após conversão para RGB: {same_shape}")

if same_shape:
    identical_pixels = np.array_equal(pixels_a, pixels_b)

    # Converte antes de subtrair para evitar estouro dos valores uint8.
    difference = np.abs(
        pixels_a.astype(np.int16) - pixels_b.astype(np.int16)
    )

    print(f"Pixels RGB exatamente iguais: {identical_pixels}")
    print(f"Maior diferença por canal: {int(difference.max())}")
    print(f"Diferença absoluta média: {difference.mean():.6f}")

else:
    print("Comparação direta dos pixels não realizada: dimensões diferentes.")


# Testa a hipótese de inversão vertical de B.
pixels_b_flipped = np.flipud(pixels_b)

if pixels_a.shape == pixels_b_flipped.shape:
    flipped_difference = np.abs(
        pixels_a.astype(np.int16)
        - pixels_b_flipped.astype(np.int16)
    )

    print(
        "Pixels iguais após inversão vertical de B:",
        np.array_equal(pixels_a, pixels_b_flipped),
    )
    print(
        "Diferença média após inversão vertical:",
        f"{flipped_difference.mean():.6f}",
    )

fig, axes = plt.subplots(1, 2, figsize=(14, 7))

axes[0].imshow(pixels_a)
axes[0].set_title("A — original")

axes[1].imshow(pixels_b_flipped)
axes[1].set_title("B — invertida verticalmente apenas para comparação")

for axis in axes:
    axis.axis("off")

plt.tight_layout()
plt.show()
plt.close(fig)

# Rotaciona B apenas em memória, sem alterar o arquivo.
pixels_b_rotated = np.rot90(pixels_b, k=2)

if pixels_a.shape == pixels_b_rotated.shape:
    rotated_difference = np.abs(
        pixels_a.astype(np.int16)
        - pixels_b_rotated.astype(np.int16)
    )

    print(
        "Pixels iguais após rotação de 180°:",
        np.array_equal(pixels_a, pixels_b_rotated),
    )
    print(
        "Diferença média após rotação de 180°:",
        f"{rotated_difference.mean():.6f}",
    )

fig, axes = plt.subplots(1, 2, figsize=(14, 7))

axes[0].imshow(pixels_a)
axes[0].set_title("A — original")

axes[1].imshow(pixels_b_rotated)
axes[1].set_title("B — rotação de 180° apenas para comparação")

for axis in axes:
    axis.axis("off")

plt.tight_layout()
plt.show()
plt.close(fig)

# --- Visualização ------------------------------------------------------------
fig, axes = plt.subplots(1, 2, figsize=(14, 7))

for axis, relative_path, pixels in zip(
    axes,
    comparison_paths,
    [pixels_a, pixels_b],
):
    height, width = pixels.shape[:2]

    axis.imshow(pixels)
    axis.set_title(
        f"{relative_path}\n{width} × {height} pixels",
        fontsize=10,
    )
    axis.axis("off")

fig.suptitle(
    "Comparação de representantes de dois grupos SHA-256\n"
    "Avaliação visual e decisão pendentes"
)

plt.tight_layout(rect=(0, 0, 1, 0.92))

print("As métricas não geram decisões automáticas de aprovação ou exclusão.")
print("Nenhuma imagem foi copiada, alterada ou excluída.")
print("Feche a janela para encerrar.")

plt.show()
plt.close(fig)


# Este registro se refere somente ao par já inspecionado.
expected_paths = [
    "Testing/notumor/Te-no_113.jpg",
    "Testing/notumor/Te-no_140.jpg",
]

if comparison_paths != expected_paths:
    raise ValueError(
        "Este registro foi preparado apenas para Te-no_113 e Te-no_140."
    )

if pixels_a.shape != pixels_b.shape:
    raise ValueError("As dimensões mudaram; revise o caso antes de registrar.")

# Recalcula as métricas para registrar os resultados desta execução.
original_difference = np.abs(
    pixels_a.astype(np.int16) - pixels_b.astype(np.int16)
)

rotated_b = np.rot90(pixels_b, k=2)
rotated_difference = np.abs(
    pixels_a.astype(np.int16) - rotated_b.astype(np.int16)
)

review_record = pd.DataFrame([{
    "component_id": COMPONENT_ID,
    "relative_path_a": comparison_paths[0],
    "relative_path_b": comparison_paths[1],
    "transformation_tested": "rotacao_180_graus_em_b",
    "mean_difference_original": float(original_difference.mean()),
    "mean_difference_rotated": float(rotated_difference.mean()),
    "identical_rgb_after_rotation": bool(
        np.array_equal(pixels_a, rotated_b)
    ),
    "visual_assessment": "provavel_duplicata_visual_com_rotacao",
    "review_basis": "inspecao_visual_e_comparacao_rgb",
    "observation": (
        "Forte alinhamento visual apos rotacao de 180 graus. "
        "Persistem diferencas nos pixels."
    ),
    "decision": "pendente",
}])

output_dir = stage_dir / "resultados"
output_dir.mkdir(parents=True, exist_ok=True)

review_path = output_dir / "revisao_te_no_113_te_no_140.csv"

if review_path.exists():
    print(f"Registro existente preservado: {review_path}")
else:
    review_record.to_csv(
        review_path,
        index=False,
        encoding="utf-8-sig",
    )
    print(f"Revisão salva em: {review_path}")

print("Controle de curadoria e imagens originais preservados.")