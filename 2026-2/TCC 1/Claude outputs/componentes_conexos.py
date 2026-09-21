"""Etapa 2: agrupamento de imagens perceptualmente parecidas em componentes conexos.

Duas imagens são ligadas se tiverem pHash igual OU dHash igual (distância
zero, T=0). Componentes conexos capturam cadeias transitivas: se A~B e B~C,
as três caem no mesmo componente, mesmo sem A e C terem hash igual entre si.
Nenhum arquivo de imagem é modificado por este script.
"""
from pathlib import Path

import networkx as nx
import pandas as pd

# As etapas são pastas irmãs, tanto no ZIP extraído quanto em Desenvolvimento.
MANIFEST_PATH = (
    Path(__file__).resolve().parent.parent
    / "Etapa 1 - Auditoria da Base" / "resultados" / "manifesto_base_dados.csv"
)

# Caso exploratório fixo: a imagem apareceu em 5 dos 6 pares com classe
# diferente detectados em pares_t0_para_revisao.csv, então seu componente
# ganha um relatório dedicado além do resumo geral.
TARGET_PATH = "Testing/glioma/Te-gl_68.jpg"

TOP_N_PRIORITY = 10


def print_section(title: str) -> None:
    print(f"\n{'=' * 76}\n{title}\n{'=' * 76}")


def run() -> None:
    print_section("ETAPA 2 - COMPONENTES CONEXOS (T=0)")
    print("Nó = imagem. Aresta = pHash igual OU dHash igual (distância zero).")
    print("Componentes conexos agrupam cadeias transitivas: A~B e B~C juntam as três.")

    df = pd.read_csv(
        MANIFEST_PATH,
        encoding="utf-8-sig",
        dtype={
            "sha256_hash": "string",
            "phash": "string",
            "dhash": "string",
        },
    )

    print_section("1. MANIFESTO DE ENTRADA")
    print(f"Imagens registradas: {len(df)}")

    if df["relative_path"].isna().any() or df["relative_path"].duplicated().any():
        raise ValueError("Cada imagem deve ter um caminho relativo preenchido e exclusivo.")
    if df[["split_assigned", "class_label"]].isna().any().any():
        raise ValueError("Split e classe devem estar preenchidos em todas as linhas.")

    missing_hash_mask = df[["phash", "dhash"]].isna().any(axis=1)
    print(f"Sem pHash ou dHash (ficam isoladas no grafo): {int(missing_hash_mask.sum())}")

    print_section("2. CONSTRUÇÃO DO GRAFO")
    graph = nx.Graph()
    # Inclui todas as imagens, mesmo as que não se ligarem a nenhuma outra.
    graph.add_nodes_from(df["relative_path"])

    edges_added = 0
    for hash_column in ["phash", "dhash"]:
        for _, group in df.dropna(subset=[hash_column]).groupby(hash_column):
            paths = group["relative_path"].tolist()
            if len(paths) < 2:
                continue

            # Ligar todos ao primeiro é suficiente para formar o componente;
            # não precisamos das arestas entre todos os pares do grupo.
            first_path = paths[0]
            for other_path in paths[1:]:
                graph.add_edge(first_path, other_path)
                edges_added += 1

    print(f"Nós no grafo (imagens): {graph.number_of_nodes()}")
    print(f"Arestas adicionadas (ligações diretas por hash igual): {edges_added}")

    print_section("3. COMPONENTES CONEXOS EM T=0")
    components = list(nx.connected_components(graph))
    candidate_components = [component for component in components if len(component) > 1]
    isolated_count = len(components) - len(candidate_components)

    print(f"Componentes totais: {len(components)}")
    print(f"  Isolados (1 imagem, sem hash repetido): {isolated_count}")
    print(f"  Com 2 ou mais imagens: {len(candidate_components)}")

    metadata = df.set_index("relative_path")
    # Ordenação estável para esta versão do grafo.
    ordered_components = sorted(candidate_components, key=lambda component: min(component))

    summary_records = []
    for index, component in enumerate(ordered_components, start=1):
        members = metadata.loc[sorted(component)]
        summary_records.append({
            "component_id": f"t0_{index:04d}",
            "image_count": len(members),
            "split_count": members["split_assigned"].nunique(),
            "class_count": members["class_label"].nunique(),
            "contains_reviewed_case": TARGET_PATH in component,
        })

    summary_df = pd.DataFrame(summary_records)
    summary_df["cross_split"] = summary_df["split_count"] > 1
    summary_df["different_class"] = summary_df["class_count"] > 1

    print(f"Imagens envolvidas nesses componentes: {int(summary_df['image_count'].sum())}")
    print(f"Componentes entre splits (train/test): {int(summary_df['cross_split'].sum())}")
    print(f"Componentes com classes diferentes: {int(summary_df['different_class'].sum())}")
    print("As duas categorias podem se sobrepor num mesmo componente.")

    # Prioriza conflitos de classe, depois componentes entre splits, depois tamanho.
    summary_df = summary_df.sort_values(
        ["different_class", "cross_split", "image_count", "component_id"],
        ascending=[False, False, False, True],
    )

    print_section("4. CASOS PRIORITÁRIOS PARA REVISÃO")
    print(f"Top {TOP_N_PRIORITY}, ordenado por: classes diferentes > entre splits > tamanho.")
    print(
        summary_df.head(TOP_N_PRIORITY)
        .rename(columns={
            "component_id": "Componente", "image_count": "Imagens",
            "split_count": "Splits", "class_count": "Classes",
            "contains_reviewed_case": "Caso Te-gl_68",
            "cross_split": "Entre splits", "different_class": "Classes diferentes",
        })
        .to_string(index=False)
    )

    conflicting_df = summary_df.loc[summary_df["different_class"]]
    if conflicting_df.empty:
        print("\nNenhum componente mistura classes diferentes.")
    else:
        print(
            f"\n{len(conflicting_df)} componente(s) misturam classes diferentes "
            "— prioridade máxima de inspeção."
        )

    print_section("5. CASO EXPLORATÓRIO — COMPONENTE DE Te-gl_68.jpg")
    target_component = nx.node_connected_component(graph, TARGET_PATH)
    print(f"Imagens no componente: {len(target_component)}")
    for relative_path in sorted(target_component):
        print(f"  {relative_path}")

    # Identifica este caso exploratório; não é uma decisão de exclusão.
    component_df = df.loc[df["relative_path"].isin(target_component)].copy()
    component_df = component_df.sort_values("relative_path")
    component_df["review_case"] = "caso_te_gl_68_t0"
    component_df["connection_rule"] = "phash_igual_ou_dhash_igual"
    component_df["decision"] = "pendente"

    # Mantém os mesmos identificadores utilizados no resumo.
    membership_records = []
    for index, component in enumerate(ordered_components, start=1):
        component_id = f"t0_{index:04d}"
        for relative_path in sorted(component):
            membership_records.append({
                "component_id": component_id,
                "relative_path": relative_path,
            })

    membership_df = pd.DataFrame(membership_records).merge(
        df, on="relative_path", how="left", validate="one_to_one"
    )

    output_dir = Path(__file__).resolve().parent / "resultados"
    output_dir.mkdir(parents=True, exist_ok=True)

    summary_path = output_dir / "resumo_componentes_t0.csv"
    membership_path = output_dir / "imagens_por_componente_t0.csv"
    component_path = output_dir / "componente_te_gl_68_t0.csv"

    summary_df.to_csv(summary_path, index=False, encoding="utf-8-sig")
    membership_df.to_csv(membership_path, index=False, encoding="utf-8-sig")
    component_df.to_csv(component_path, index=False, encoding="utf-8-sig")

    print_section("6. RELATÓRIOS GERADOS")
    print(f"Pasta: {output_dir}")
    for path, description in [
        (summary_path, "uma linha por componente com 2+ imagens"),
        (membership_path, "uma linha por imagem, com o componente a que pertence"),
        (component_path, f"caso exploratório: {len(component_df)} imagens do componente de Te-gl_68.jpg"),
    ]:
        print(f"  {path.name}: {description}")

    print_section("RESUMO FINAL")
    print(f"Imagens no grafo: {graph.number_of_nodes()}")
    print(f"Componentes com 2+ imagens: {len(candidate_components)}")
    print(f"Componentes entre splits: {int(summary_df['cross_split'].sum())}")
    print(f"Componentes com classes diferentes: {int(summary_df['different_class'].sum())}")
    print("Nenhuma imagem foi removida, convertida ou reclassificada.")
    print("Próximo passo: inspeção humana e definição do T final.")


if __name__ == "__main__":
    run()
