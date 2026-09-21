from pathlib import Path
import pandas as pd

import matplotlib.pyplot as plt
from PIL import Image

# As pastas das etapas ficam dentro de Desenvolvimento.
development_dir = Path(__file__).resolve().parent.parent

review_path = (
    development_dir
    / "Etapa 2 - Deduplicacao e Sensibilidade"
    / "resultados"
    / "pares_t0_para_revisao.csv"
)

review_df = pd.read_csv(review_path, encoding="utf-8-sig")

# Seleciona os candidatos cujas duas imagens têm rótulos diferentes.
class_conflicts_df = review_df.loc[
    review_df["class_a"] != review_df["class_b"]
].copy()

print(f"Pares candidatos carregados: {len(review_df)}")
print(f"Pares com classes diferentes: {len(class_conflicts_df)}")

print(
    class_conflicts_df[
        [
            "relative_path_a",
            "relative_path_b",
            "class_a",
            "class_b",
            "phash_distance",
            "dhash_distance",
        ]
    ].to_string(index=False)
)

dataset_dir = development_dir.parent / "Brain Tumor MRI Dataset"

output_dir = Path(__file__).resolve().parent / "resultados"
output_dir.mkdir(parents=True, exist_ok=True)

review_log_path = output_dir / "revisao_inicial_classes_diferentes.csv"

# Preserva os caminhos, classes, splits e distâncias de cada par.
review_log_df = class_conflicts_df.copy()

review_log_df["visual_assessment"] = "provavel_duplicata_visual"
review_log_df["review_basis"] = "comparacao_visual_das_capturas"
review_log_df["observation"] = (
    "Contornos e detalhes visuais muito semelhantes. "
    "Rotulos conflitantes; igualdade de pixels nao verificada."
)
review_log_df["decision"] = "pendente"

# Evita sobrescrever decisões editadas posteriormente.
if review_log_path.exists():
    print(f"Registro já existe e foi preservado: {review_log_path}")
else:
    review_log_df.to_csv(
        review_log_path,
        index=False,
        encoding="utf-8-sig",
    )
    print(f"Revisões registradas: {len(review_log_df)}")
    print(f"Registro salvo em: {review_log_path}")

# Confere o registro salvo em disco.
saved_review_df = pd.read_csv(
    review_log_path,
    encoding="utf-8-sig",
)

print(f"\nRevisões no arquivo salvo: {len(saved_review_df)}")
print(
    saved_review_df[
        ["relative_path_a", "relative_path_b", "visual_assessment", "decision"]
    ].to_string(index=False)
)