"""Migra as 221 revisões manuais já feitas (fluxo antigo, componente t0_0134)
para o formato único de revisões usado por `curadoria.revisao`.

Isso existe para não jogar fora um trabalho manual real: essas 221 linhas
foram avaliadas par a par, com botão clicado por humano, na ferramenta
antiga (interface_botoes.py). São dados legítimos, não devem ser
re-revisados do zero.

Uso:
    python -m migracao.migrar_revisoes_antigas \
        --legado migracao/dados_legados/t0_0134_revisoes_consolidadas.csv \
        --saida resultados/revisoes.csv \
        --clusters resultados/clusters.csv   # opcional, para já preencher cluster_id novo

Sem --clusters, a migração ainda funciona; cluster_id fica em branco e pode
ser preenchido depois com `preencher_cluster_id`, assim que a nova etapa de
`cluster` tiver rodado sobre o pipeline consolidado.
"""
from __future__ import annotations

import argparse
from pathlib import Path

import pandas as pd

# Só migramos avaliações que já eram, de fato, um clique humano na
# ferramenta de botões — não os campos herdados de scripts anteriores
# (ex.: "provavel_duplicata_visual" escrito sem revisão real).
ORIGENS_HUMANAS_VALIDAS = {
    "inspecao_visual_com_botoes",
    "inspecao_visual_interativa_padronizada",
    "inspecao_visual_da_comparacao_padronizada",
}


def migrar(legado_path: Path, clusters_path: Path | None) -> pd.DataFrame:
    legado = pd.read_csv(legado_path, encoding="utf-8-sig")

    humanas = legado[legado["review_basis"].isin(ORIGENS_HUMANAS_VALIDAS)].copy()
    ignoradas = len(legado) - len(humanas)
    if ignoradas:
        print(
            f"Aviso: {ignoradas} linha(s) do arquivo legado NÃO foram migradas "
            "por não terem origem claramente humana (review_basis diferente do "
            "esperado). Revise manualmente se necessário."
        )

    cluster_map: dict[str, str] = {}
    if clusters_path is not None and clusters_path.exists():
        clusters = pd.read_csv(clusters_path, encoding="utf-8-sig")
        cluster_map = clusters.set_index("relative_path")["cluster_id"].to_dict()

    migradas = pd.DataFrame({
        "cluster_id": humanas["relative_path_a"].map(cluster_map).fillna(""),
        "relative_path_a": humanas["relative_path_a"],
        "relative_path_b": humanas["relative_path_b"],
        "mean_absolute_difference": humanas["mean_absolute_difference"],
        "ssim": humanas["ssim"],
        "best_rotation_b": humanas["rotation_b_degrees"],
        "sugestao_revisao": "",
        "visual_assessment": humanas["visual_assessment"],
        "observation": humanas["observation"].fillna(""),
        "decision": "pendente",
        "reviewed_at": humanas["reviewed_at"],
    })

    if not cluster_map:
        print(
            "Aviso: nenhum mapeamento de cluster novo fornecido — cluster_id "
            "ficou em branco nas linhas migradas. Rode 'curadoria cluster' no "
            "pipeline novo e reaplique o mapeamento antes de usar a fila de "
            "revisão, ou os pares migrados não serão associados a nenhum grupo."
        )

    return migradas


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--legado", type=Path, required=True)
    parser.add_argument("--saida", type=Path, required=True)
    parser.add_argument("--clusters", type=Path, default=None)
    args = parser.parse_args()

    migradas = migrar(args.legado, args.clusters)

    if args.saida.exists():
        existentes = pd.read_csv(args.saida, encoding="utf-8-sig")
        chaves_existentes = set(
            zip(existentes["relative_path_a"], existentes["relative_path_b"])
        )
        novas = migradas[
            ~migradas.apply(
                lambda r: (r["relative_path_a"], r["relative_path_b"]) in chaves_existentes,
                axis=1,
            )
        ]
        resultado = pd.concat([existentes, novas], ignore_index=True)
    else:
        resultado = migradas

    args.saida.parent.mkdir(parents=True, exist_ok=True)
    resultado.to_csv(args.saida, index=False, encoding="utf-8-sig")
    print(f"{len(migradas)} revisões legadas migradas.")
    print(f"Total no arquivo de revisões agora: {len(resultado)}")
    print(f"Salvo em: {args.saida}")


if __name__ == "__main__":
    main()
