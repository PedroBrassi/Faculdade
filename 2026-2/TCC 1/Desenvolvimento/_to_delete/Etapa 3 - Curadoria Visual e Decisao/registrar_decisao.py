from pathlib import Path

import pandas as pd

stage_dir = Path(__file__).resolve().parent
development_dir = stage_dir.parent

component_path = (
    development_dir
    / "Etapa 2 - Deduplicacao e Sensibilidade"
    / "resultados"
    / "componente_te_gl_68_t0.csv"
)

component_df = pd.read_csv(
    component_path,
    encoding="utf-8-sig",
    dtype={"sha256_hash": "string"},
)

# Confere exatamente quais arquivos receberam a decisão humana.
expected_paths = {
    "Testing/glioma/Te-gl_68.jpg",
    "Testing/meningioma/Te-me_50.jpg",
    "Testing/meningioma/Te-me_61.jpg",
    "Testing/meningioma/Te-me_96.jpg",
    "Training/meningioma/Tr-me_458.jpg",
    "Training/meningioma/Tr-me_468.jpg",
    "Training/meningioma/Tr-me_714.jpg",
}

if (
    len(component_df) != 7
    or set(component_df["relative_path"]) != expected_paths
):
    raise ValueError("O componente não corresponde às sete imagens revisadas.")

decisions_df = component_df[
    ["relative_path", "sha256_hash", "split_assigned", "class_label"]
].copy()

decisions_df["review_case"] = "caso_te_gl_68_t0"
decisions_df["decision"] = "excluir_da_base_curada"
decisions_df["reason"] = (
    "Provavel duplicacao visual com rotulos conflitantes; "
    "exclusao por ambiguidade, confirmada pelo responsavel."
)
decisions_df["original_action"] = "preservar"

output_dir = stage_dir / "resultados"
output_dir.mkdir(parents=True, exist_ok=True)

decision_path = output_dir / "decisao_te_gl_68_t0.csv"

# Não sobrescreve um registro de decisão existente.
if decision_path.exists():
    print(f"Decisão já registrada; arquivo preservado: {decision_path}")
else:
    decisions_df.to_csv(
        decision_path,
        index=False,
        encoding="utf-8-sig",
    )
    print(f"Exclusões da futura base registradas: {len(decisions_df)}")
    print(f"Decisão salva em: {decision_path}")

print("Nenhuma imagem original foi modificada ou removida.")