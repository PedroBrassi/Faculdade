"""Agrupa pares candidatos (camada 3) em componentes conexos.

Diferença importante em relação a uma versão anterior deste módulo:
o agrupamento aqui parte diretamente dos pares candidatos (camada 3,
já filtrados por distância de Hamming), e NÃO de um subconjunto
"confirmado" por um limiar automático de SSIM. Ou seja, o agrupamento é
estrutural (quem é candidato a duplicata de quem), e a pontuação de
MAE/SSIM (`score.py`) só entra depois, para priorizar a ORDEM de revisão
dentro de cada grupo — nunca para decidir quem entra no grupo.

Isso segue a mesma lógica do pré-projeto: agrupar tudo que é candidato,
revisar por amostragem, nunca excluir automaticamente.
"""
from __future__ import annotations

import pandas as pd


class _UF:
    def __init__(self) -> None:
        self.parent: dict[str, str] = {}

    def find(self, x: str) -> str:
        self.parent.setdefault(x, x)
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def union(self, a: str, b: str) -> None:
        ra, rb = self.find(a), self.find(b)
        if ra != rb:
            self.parent[ra] = rb


def cluster_candidates(pairs: pd.DataFrame, inventory: pd.DataFrame) -> pd.DataFrame:
    """Agrupa TODOS os pares candidatos em componentes conexos.

    `pairs`: saída da camada 3 (colunas relative_path_a/b no mínimo),
    com ou sem as colunas de pontuação de `score.py`.

    Retorna uma linha por imagem pertencente a algum grupo, com:
    cluster_id, image_count, splits, classes, cross_split, cross_class.
    """
    uf = _UF()
    for row in pairs.itertuples():
        uf.union(row.relative_path_a, row.relative_path_b)

    roots: dict[str, str] = {}
    for path in list(uf.parent):
        roots[path] = uf.find(path)

    # Id numérico estável por ordem alfabética do representante do grupo.
    root_to_id: dict[str, str] = {}
    for root in sorted(set(roots.values())):
        root_to_id[root] = f"c_{len(root_to_id) + 1:04d}"

    cluster_id = {path: root_to_id[root] for path, root in roots.items()}

    df = pd.DataFrame({
        "relative_path": list(cluster_id),
        "cluster_id": list(cluster_id.values()),
    })

    meta = inventory.set_index("relative_path")[["split", "class_label"]]
    df = df.join(meta, on="relative_path")

    agg = df.groupby("cluster_id").agg(
        image_count=("relative_path", "size"),
        splits=("split", lambda s: "|".join(sorted(set(s)))),
        classes=("class_label", lambda s: "|".join(sorted(set(s)))),
    ).reset_index()

    agg["cross_split"] = agg["splits"].str.contains(r"\|")
    agg["cross_class"] = agg["classes"].str.contains(r"\|")

    return df.merge(agg, on="cluster_id", how="left")


def priority_order(scored_pairs: pd.DataFrame) -> pd.DataFrame:
    """Ordena pares pontuados por prioridade de revisão manual.

    Prioriza: classes diferentes primeiro (maior risco), depois cross-split,
    depois maior SSIM (mais parecido primeiro). É só ordenação de fila —
    não filtra nenhum par.
    """
    cols = [c for c in ("different_class", "cross_split", "ssim") if c in scored_pairs.columns]
    ascending = [False] * len(cols)
    if "ssim" in cols:
        ascending[cols.index("ssim")] = False
    return scored_pairs.sort_values(cols, ascending=ascending) if cols else scored_pairs
