"""CLI único da curadoria: inventário, detecção, pontuação, agrupamento,
revisão manual e registro de decisões.

Substitui os scripts soltos que existiam antes (um por etapa, vários com
lógica duplicada). Cada subcomando é um passo do pipeline; nenhum passo
decide sozinho o que é duplicata — isso é sempre humano (ver `decisao.py`).
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import pandas as pd
from tqdm import tqdm

from .canonical import canonical_content_id, compute_variant_hashes
from .cluster import cluster_candidates
from .decisao import aplicar_decisoes_em_lote, registrar_decisao, resumo_para_decisao
from .detect import layer1_exact_sha256, layer2_content, layer3_perceptual
from .revisao import JanelaRevisao, carregar_revisoes, montar_fila
from .score import score_pairs

IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg", ".bmp", ".tif", ".tiff"}


def iter_images(root: Path):
    for path in sorted(root.rglob("*")):
        if path.is_file() and path.suffix.lower() in IMAGE_EXTENSIONS:
            yield path


def build_inventory(dataset: Path, output: Path) -> None:
    """Inventário + hashes canônicos (camada 0)."""
    paths = list(iter_images(dataset))
    if not paths:
        raise SystemExit(f"Nenhuma imagem encontrada em {dataset}")

    print(f"Encontrados {len(paths)} arquivos em {dataset}")
    records: list[dict] = []

    for path in tqdm(paths, desc="Inventário", unit="img"):
        rel = path.relative_to(dataset).as_posix()
        parts = rel.split("/")
        split = parts[0] if len(parts) >= 3 else "unknown"
        cls = parts[1] if len(parts) >= 3 else "unknown"

        record = {
            "relative_path": rel,
            "split": split,
            "class_label": cls,
            "file_size": path.stat().st_size,
            "valid": False,
            "error": "",
            "file_sha256": "",
            "canonical_content_id": "",
        }
        try:
            record["file_sha256"] = hashlib.sha256(path.read_bytes()).hexdigest()
            hashes = compute_variant_hashes(path)
            record["canonical_content_id"] = canonical_content_id(hashes)
            for name, h in hashes.items():
                record[f"phash_{name}"] = h.phash
                record[f"dhash_{name}"] = h.dhash
            record["valid"] = True
        except Exception as exc:  # noqa: BLE001 — registra e segue
            record["error"] = f"{type(exc).__name__}: {exc}"
        records.append(record)

    df = pd.DataFrame(records)
    output.parent.mkdir(parents=True, exist_ok=True)
    df.to_csv(output, index=False, encoding="utf-8-sig")
    valid = int(df["valid"].sum())
    print(f"Inventário salvo: {output}")
    print(f"  válidas: {valid} | falhas: {len(df) - valid}")


def load_inventory(path: Path) -> pd.DataFrame:
    df = pd.read_csv(path, encoding="utf-8-sig", dtype=str, keep_default_na=False)
    df["valid"] = df["valid"].str.lower().eq("true")
    df["file_size"] = pd.to_numeric(df["file_size"], errors="coerce").fillna(0).astype("int64")
    return df


def run_detect(inventory: Path, output_dir: Path, max_distance: int, require_both: bool) -> dict:
    inv = load_inventory(inventory)
    output_dir.mkdir(parents=True, exist_ok=True)

    print("Camada 1 — SHA-256 exato...")
    l1 = layer1_exact_sha256(inv)
    l1.to_csv(output_dir / "camada1_sha256.csv", index=False, encoding="utf-8-sig")

    print("Camada 2 — Conteúdo (canonical_content_id, invariante a rotação/reflexão)...")
    l2 = layer2_content(inv)
    l2.to_csv(output_dir / "camada2_conteudo.csv", index=False, encoding="utf-8-sig")

    print(f"Camada 3 — Candidatos perceptuais (T={max_distance})...")
    l3 = layer3_perceptual(inv, max_distance=max_distance, require_both_hashes=require_both)
    l3.to_csv(output_dir / "camada3_candidatos.csv", index=False, encoding="utf-8-sig")

    if not l3.empty:
        cross = l3[l3["cross_split"]]
        cross.to_csv(output_dir / "vazamento_treino_teste_candidatos.csv", index=False, encoding="utf-8-sig")

    valid = inv[inv["valid"]]
    summary = {
        "input_inventory": str(inventory),
        "total_images": len(inv),
        "valid_images": int(len(valid)),
        "invalid_images": int(len(inv) - len(valid)),
        "layer1_groups_sha256": int(l1["file_sha256"].nunique()) if not l1.empty else 0,
        "layer1_images": int(len(l1)),
        "layer2_groups_content": int(l2["canonical_content_id"].nunique()) if not l2.empty else 0,
        "layer2_images": int(len(l2)),
        "layer3_pairs": int(len(l3)),
        "layer3_cross_split_pairs": int(l3["cross_split"].sum()) if not l3.empty else 0,
        "layer3_different_class_pairs": int(l3["different_class"].sum()) if not l3.empty else 0,
        "parameters": {"max_distance": max_distance, "require_both_hashes": require_both},
    }
    (output_dir / "resumo.json").write_text(
        json.dumps(summary, indent=2, ensure_ascii=False), encoding="utf-8",
    )
    print("\nRESUMO")
    for k, v in summary.items():
        if k != "parameters":
            print(f"  {k:<34} {v}")
    return summary


def build_controle(inventory: Path, output: Path) -> None:
    """Inicializa o controle mestre de curadoria — todas as imagens 'pendente'.

    Nunca marca nada como excluída aqui. Isso só acontece via `decidir`.
    """
    inv = load_inventory(inventory)
    controle = inv.copy()
    controle["curation_status"] = "pendente"
    controle["curation_reason"] = ""
    controle["decided_at"] = ""
    output.parent.mkdir(parents=True, exist_ok=True)
    controle.to_csv(output, index=False, encoding="utf-8-sig")
    print(f"Controle inicializado com {len(controle)} imagens, todas 'pendente'.")
    print(f"Salvo em: {output}")


def main() -> None:
    parser = argparse.ArgumentParser(prog="curadoria", description=__doc__)
    sub = parser.add_subparsers(dest="cmd", required=True)

    p_inv = sub.add_parser("inventory", help="Constrói o inventário com hashes canônicos")
    p_inv.add_argument("--dataset", type=Path, required=True)
    p_inv.add_argument("--output", type=Path, default=Path("resultados/inventario.csv"))

    p_det = sub.add_parser("detect", help="Executa as 3 camadas de detecção")
    p_det.add_argument("--inventory", type=Path, default=Path("resultados/inventario.csv"))
    p_det.add_argument("--output", type=Path, default=Path("resultados"))
    p_det.add_argument("--max-distance", type=int, default=4)
    p_det.add_argument("--require-both", action="store_true")

    p_score = sub.add_parser("score", help="Calcula MAE/SSIM por par candidato (não decide nada)")
    p_score.add_argument("--dataset", type=Path, required=True)
    p_score.add_argument("--pairs", type=Path, required=True)
    p_score.add_argument("--output", type=Path, required=True)

    p_clu = sub.add_parser("cluster", help="Agrupa pares candidatos em componentes conexos")
    p_clu.add_argument("--pairs", type=Path, required=True)
    p_clu.add_argument("--inventory", type=Path, required=True)
    p_clu.add_argument("--output", type=Path, required=True)

    p_ctrl = sub.add_parser("init-controle", help="Cria o controle mestre (todas 'pendente')")
    p_ctrl.add_argument("--inventory", type=Path, required=True)
    p_ctrl.add_argument("--output", type=Path, required=True)

    p_rev = sub.add_parser("revisar", help="Abre a janela de revisão manual")
    p_rev.add_argument("--dataset", type=Path, required=True)
    p_rev.add_argument("--pontuados", type=Path, required=True, help="CSV de saída do comando score")
    p_rev.add_argument("--clusters", type=Path, default=None, help="CSV de saída do comando cluster (opcional)")
    p_rev.add_argument("--saida", type=Path, required=True, help="CSV único de revisões")

    p_resumo = sub.add_parser("resumo-decisao", help="Relatório por cluster para apoiar decisão humana")
    p_resumo.add_argument("--revisoes", type=Path, required=True)
    p_resumo.add_argument("--output", type=Path, required=True)

    p_dec = sub.add_parser("decidir", help="Registra a decisão de UMA imagem (ato humano obrigatório)")
    p_dec.add_argument("--controle", type=Path, required=True)
    p_dec.add_argument("--relative-path", type=str, required=True)
    p_dec.add_argument("--decision", type=str, required=True, choices=["mantida", "excluida"])
    p_dec.add_argument("--reason", type=str, required=True)

    p_lote = sub.add_parser("decidir-lote", help="Aplica um CSV de decisões escrito à mão")
    p_lote.add_argument("--controle", type=Path, required=True)
    p_lote.add_argument("--decisoes", type=Path, required=True)

    args = parser.parse_args()

    if args.cmd == "inventory":
        build_inventory(args.dataset, args.output)
    elif args.cmd == "detect":
        run_detect(args.inventory, args.output, args.max_distance, args.require_both)
    elif args.cmd == "score":
        score_pairs(args.dataset, args.pairs, args.output)
    elif args.cmd == "cluster":
        inv = load_inventory(args.inventory)
        pairs = pd.read_csv(args.pairs, encoding="utf-8-sig")
        df = cluster_candidates(pairs, inv)
        args.output.parent.mkdir(parents=True, exist_ok=True)
        df.to_csv(args.output, index=False, encoding="utf-8-sig")
        print(f"Imagens em grupos candidatos: {len(df)}")
        print(f"Grupos: {df['cluster_id'].nunique()}")
        print(f"Grupos cross-split: {int(df.drop_duplicates('cluster_id')['cross_split'].sum())}")
        print(f"Grupos cross-class: {int(df.drop_duplicates('cluster_id')['cross_class'].sum())}")
    elif args.cmd == "init-controle":
        build_controle(args.inventory, args.output)
    elif args.cmd == "revisar":
        scored = pd.read_csv(args.pontuados, encoding="utf-8-sig")
        if args.clusters:
            clusters = pd.read_csv(args.clusters, encoding="utf-8-sig")
            path_to_cluster = clusters.set_index("relative_path")["cluster_id"]
            scored["cluster_id"] = scored["relative_path_a"].map(path_to_cluster)
        existentes = carregar_revisoes(args.saida)
        fila = montar_fila(scored, existentes)
        print(f"Pares pendentes de revisão: {len(fila)}")
        if not fila:
            print("Nada para revisar.")
            return
        janela = JanelaRevisao(fila, args.dataset, args.saida)
        janela.mostrar()
    elif args.cmd == "resumo-decisao":
        revisoes = pd.read_csv(args.revisoes, encoding="utf-8-sig")
        resumo = resumo_para_decisao(revisoes)
        args.output.parent.mkdir(parents=True, exist_ok=True)
        resumo.to_csv(args.output, index=False, encoding="utf-8-sig")
        print(f"{len(resumo)} pares avaliados como prováveis duplicatas, agrupados por cluster.")
        print(f"Relatório salvo em: {args.output}")
        print("Nenhuma decisão foi tomada — use 'decidir' ou 'decidir-lote'.")
    elif args.cmd == "decidir":
        registrar_decisao(args.controle, args.relative_path, args.decision, args.reason)
    elif args.cmd == "decidir-lote":
        aplicar_decisoes_em_lote(args.controle, args.decisoes)


if __name__ == "__main__":
    main()
