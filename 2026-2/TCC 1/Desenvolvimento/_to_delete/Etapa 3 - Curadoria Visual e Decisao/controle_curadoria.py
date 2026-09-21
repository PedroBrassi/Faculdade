from pathlib import Path

import pandas as pd

stage_dir = Path(__file__).resolve().parent
development_dir = stage_dir.parent

manifest_path = (
    development_dir
    / "Etapa 1 - Auditoria da Base"
    / "resultados"
    / "manifesto_base_dados.csv"
)

decision_path = stage_dir / "resultados" / "decisao_te_gl_68_t0.csv"
control_path = stage_dir / "resultados" / "controle_curadoria.csv"

# Interrompe para não apagar futuras decisões adicionadas ao controle.
if control_path.exists():
    raise FileExistsError(
        f"O controle já existe e foi preservado: {control_path}"
    )

manifest_df = pd.read_csv(
    manifest_path,
    encoding="utf-8-sig",
    dtype={
        "sha256_hash": "string",
        "phash": "string",
        "dhash": "string",
    },
)

decisions_df = pd.read_csv(
    decision_path,
    encoding="utf-8-sig",
    dtype={"sha256_hash": "string"},
)

# Confere se cada decisão corresponde ao arquivo auditado.
if (
    manifest_df["relative_path"].isna().any()
    or manifest_df["relative_path"].duplicated().any()
    or decisions_df["relative_path"].isna().any()
    or decisions_df["relative_path"].duplicated().any()
):
    raise ValueError("Há caminhos ausentes ou repetidos nos arquivos de entrada.")

checked_decisions = decisions_df.merge(
    manifest_df[["relative_path", "sha256_hash"]],
    on="relative_path",
    how="left",
    suffixes=("_decision", "_manifest"),
    validate="one_to_one",
)

if (
    checked_decisions["sha256_hash_decision"].isna().any()
    or checked_decisions["sha256_hash_manifest"].isna().any()
    or not (
        checked_decisions["sha256_hash_decision"]
        == checked_decisions["sha256_hash_manifest"]
    ).all()
):
    raise ValueError("Uma decisão não corresponde ao manifesto atual.")

if not decisions_df["decision"].eq("excluir_da_base_curada").all():
    raise ValueError("O arquivo contém uma decisão diferente da esperada.")

# Todos começam pendentes; somente decisões registradas alteram o status.
control_df = manifest_df.copy()
control_df["curation_status"] = "pendente"
control_df["curation_reason"] = ""

reasons = decisions_df.set_index("relative_path")["reason"]
excluded_mask = control_df["relative_path"].isin(reasons.index)

control_df.loc[excluded_mask, "curation_status"] = "excluida"
control_df.loc[excluded_mask, "curation_reason"] = (
    control_df.loc[excluded_mask, "relative_path"].map(reasons)
)

control_path.parent.mkdir(parents=True, exist_ok=True)
control_df.to_csv(control_path, index=False, encoding="utf-8-sig")

print("CONTROLE DE CURADORIA")

for status in ["aprovada", "excluida", "pendente"]:
    count = int(control_df["curation_status"].eq(status).sum())
    print(f"{status}: {count}")

print(f"\nControle salvo em: {control_path}")
print("Nenhuma imagem foi copiada ou alterada.")