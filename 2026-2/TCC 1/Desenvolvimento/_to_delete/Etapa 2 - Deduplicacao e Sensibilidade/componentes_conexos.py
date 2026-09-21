from pathlib import Path

import pandas as pd
import networkx as nx

development_dir = Path(__file__).resolve().parent.parent

manifest_path = (
    development_dir
    / "Etapa 1 - Auditoria da Base"
    / "resultados"
    / "manifesto_base_dados.csv"
)

df = pd.read_csv(
    manifest_path,
    encoding="utf-8-sig",
    dtype={
        "sha256_hash": "string",
        "phash": "string",
        "dhash": "string",
    },
)

print(f"Imagens carregadas: {len(df)}")
print(f"Caminhos relativos repetidos: {df['relative_path'].duplicated().sum()}")
print("Hashes perceptuais ausentes:")
print(df[["phash", "dhash"]].isna().sum())

# Inclui todas as imagens, mesmo aquelas sem conexão com outras.
graph = nx.Graph()
graph.add_nodes_from(df["relative_path"])

# Conecta imagens com pHash igual OU dHash igual.
# Cada hash é agrupado separadamente.
for hash_column in ["phash", "dhash"]:
    for _, group in df.groupby(hash_column):
        paths = group["relative_path"].tolist()

        if len(paths) < 2:
            continue

        # Ligar todos ao primeiro é suficiente para formar o componente.
        # Não precisamos armazenar todos os pares do grupo.
        first_path = paths[0]

        for other_path in paths[1:]:
            graph.add_edge(first_path, other_path)

components = list(nx.connected_components(graph))

candidate_components = [
    component
    for component in components
    if len(component) > 1
]

print(f"\nImagens no grafo: {graph.number_of_nodes()}")
print(f"Componentes com duas ou mais imagens: {len(candidate_components)}")

# Localiza o componente que contém a imagem dos seis pares revisados.
target_path = "Testing/glioma/Te-gl_68.jpg"

target_component = nx.node_connected_component(graph, target_path)

print(f"Imagens no componente de Te-gl_68.jpg: {len(target_component)}")

for relative_path in sorted(target_component):
    print(relative_path)

# Seleciona os registros das imagens pertencentes ao componente.
component_df = df.loc[
    df["relative_path"].isin(target_component)
].copy()

component_df = component_df.sort_values("relative_path")

# Identifica este caso exploratório; não é uma decisão de exclusão.
component_df["review_case"] = "caso_te_gl_68_t0"
component_df["connection_rule"] = "phash_igual_ou_dhash_igual"
component_df["decision"] = "pendente"

output_dir = Path(__file__).resolve().parent / "resultados"
output_dir.mkdir(parents=True, exist_ok=True)

component_path = output_dir / "componente_te_gl_68_t0.csv"

component_df.to_csv(
    component_path,
    index=False,
    encoding="utf-8-sig",
)

print(f"\nImagens exportadas: {len(component_df)}")
print(f"Componente salvo em: {component_path}")

# Resume os componentes para organizar a revisão humana.
metadata = df.set_index("relative_path")
component_records = []

# Ordenação estável para esta versão do grafo.
ordered_components = sorted(
    candidate_components,
    key=lambda component: min(component),
)

for index, component in enumerate(ordered_components, start=1):
    members = metadata.loc[sorted(component)]

    component_records.append({
        "component_id": f"t0_{index:04d}",
        "image_count": len(members),
        "split_count": members["split_assigned"].nunique(),
        "class_count": members["class_label"].nunique(),
        "contains_reviewed_case": (
            "Testing/glioma/Te-gl_68.jpg" in component
        ),
    })

component_summary_df = pd.DataFrame(component_records)

# Prioriza conflitos de classe, depois componentes entre splits.
component_summary_df["cross_split"] = (
    component_summary_df["split_count"] > 1
)
component_summary_df["different_class"] = (
    component_summary_df["class_count"] > 1
)

component_summary_df = component_summary_df.sort_values(
    ["different_class", "cross_split", "image_count", "component_id"],
    ascending=[False, False, False, True],
)

output_dir = Path(__file__).resolve().parent / "resultados"
output_dir.mkdir(parents=True, exist_ok=True)

summary_path = output_dir / "resumo_componentes_t0.csv"
component_summary_df.to_csv(
    summary_path,
    index=False,
    encoding="utf-8-sig",
)

print("\nCOMPONENTES CANDIDATOS EM T=0")
print(f"Total: {len(component_summary_df)}")
print(f"Entre splits: {int(component_summary_df['cross_split'].sum())}")
print(f"Com classes diferentes: {int(component_summary_df['different_class'].sum())}")
print("\nPrimeiros 10 componentes:")
print(component_summary_df.head(10).to_string(index=False))

# Mantém os mesmos identificadores utilizados no resumo.
membership_records = []

for index, component in enumerate(ordered_components, start=1):
    component_id = f"t0_{index:04d}"

    for relative_path in sorted(component):
        membership_records.append({
            "component_id": component_id,
            "relative_path": relative_path,
        })

membership_df = pd.DataFrame(membership_records)

# Acrescenta os metadados de cada imagem.
membership_df = membership_df.merge(
    df,
    on="relative_path",
    how="left",
    validate="one_to_one",
)

membership_path = output_dir / "imagens_por_componente_t0.csv"
membership_df.to_csv(
    membership_path,
    index=False,
    encoding="utf-8-sig",
)

next_component_df = membership_df.loc[
    membership_df["component_id"] == "t0_0134"
]

print("\nPRÓXIMO COMPONENTE: t0_0134")
print(f"Imagens: {len(next_component_df)}")
print(
    next_component_df.groupby(
        ["split_assigned", "class_label"]
    ).size().to_string()
)
print(f"\nMapeamento salvo em: {membership_path}")