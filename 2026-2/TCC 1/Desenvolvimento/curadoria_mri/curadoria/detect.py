"""Detecção em camadas hierárquicas."""
from __future__ import annotations

import numpy as np
import pandas as pd
from tqdm import tqdm

from .index import HammingIndex


VARIANT_NAMES = [
    "identity", "rot90", "rot180", "rot270",
    "flip_h", "flip_h_rot90", "flip_v", "flip_h_rot270",
]
PHASH_COLS = [f"phash_{n}" for n in VARIANT_NAMES]
DHASH_COLS = [f"dhash_{n}" for n in VARIANT_NAMES]


def _hex_to_uint64(series: pd.Series) -> np.ndarray:
    return np.array(
        [int(h, 16) if isinstance(h, str) and h else 0 for h in series],
        dtype=np.uint64,
    )


def _popcount(x: np.ndarray) -> np.ndarray:
    """Popcount elementwise. numpy >= 2.0 tem np.bitwise_count."""
    if hasattr(np, "bitwise_count"):
        return np.bitwise_count(x)
    # Fallback: tabela de 256 entradas, 8 bytes por uint64
    table = np.array([bin(i).count("1") for i in range(256)], dtype=np.uint8)
    view = x.view(np.uint8).reshape(*x.shape, 8)
    return table[view].sum(axis=-1).astype(np.uint64)


def layer1_exact_sha256(inv: pd.DataFrame) -> pd.DataFrame:
    valid = inv[inv["valid"]].copy()
    mask = valid["file_sha256"].duplicated(keep=False)
    return valid[mask].sort_values(["file_sha256", "relative_path"])


def layer2_content(inv: pd.DataFrame) -> pd.DataFrame:
    valid = inv[inv["valid"]].copy()
    mask = valid["canonical_content_id"].duplicated(keep=False)
    return valid[mask].sort_values(["canonical_content_id", "relative_path"])


def layer3_perceptual(
    inv: pd.DataFrame,
    max_distance: int = 4,
    require_both_hashes: bool = False,
) -> pd.DataFrame:
    valid = inv[inv["valid"]].reset_index(drop=True)
    n = len(valid)
    columns = [
        "relative_path_a", "relative_path_b",
        "split_a", "split_b", "class_a", "class_b",
        "min_phash_distance", "min_dhash_distance",
        "cross_split", "different_class",
    ]
    if n == 0:
        return pd.DataFrame(columns=columns)

    # Arrays (n, 8) de uint64 — uma coluna por variante diédrica.
    p_arr = np.stack([_hex_to_uint64(valid[c]) for c in PHASH_COLS], axis=1)
    d_arr = np.stack([_hex_to_uint64(valid[c]) for c in DHASH_COLS], axis=1)

    # Índices. A mesma imagem pode aparecer várias vezes (uma por variante);
    # o HammingIndex deduplica por bucket.
    p_index = HammingIndex(tables=8)
    d_index = HammingIndex(tables=8)
    for i in range(n):
        for v in range(8):
            p_index.add(format(int(p_arr[i, v]), "016x"), i)
            d_index.add(format(int(d_arr[i, v]), "016x"), i)

    paths = valid["relative_path"].to_numpy()
    splits = valid["split"].to_numpy()
    classes = valid["class_label"].to_numpy()

    records: list[dict] = []
    seen: set[tuple[int, int]] = set()

    for i in tqdm(range(n), desc="Camada 3", unit="img"):
        cand: set[int] = set()
        for v in range(8):
            cand.update(p_index.query(format(int(p_arr[i, v]), "016x"), max_distance))
            cand.update(d_index.query(format(int(d_arr[i, v]), "016x"), max_distance))

        for j in cand:
            if j <= i:
                continue
            key = (i, j)
            if key in seen:
                continue
            seen.add(key)

            # min sobre as 8x8 combinações via numpy (rápido).
            xor_p = p_arr[i][:, None] ^ p_arr[j][None, :]
            min_p = int(_popcount(xor_p).min())
            xor_d = d_arr[i][:, None] ^ d_arr[j][None, :]
            min_d = int(_popcount(xor_d).min())

            if require_both_hashes:
                hit = min_p <= max_distance and min_d <= max_distance
            else:
                hit = min_p <= max_distance or min_d <= max_distance
            if not hit:
                continue

            records.append({
                "relative_path_a": paths[i],
                "relative_path_b": paths[j],
                "split_a": splits[i],
                "split_b": splits[j],
                "class_a": classes[i],
                "class_b": classes[j],
                "min_phash_distance": min_p,
                "min_dhash_distance": min_d,
                "cross_split": splits[i] != splits[j],
                "different_class": classes[i] != classes[j],
            })

    return pd.DataFrame(records, columns=columns)