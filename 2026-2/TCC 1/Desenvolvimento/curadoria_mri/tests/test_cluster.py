"""Testes do agrupamento por componentes conexos e da priorização."""
import pandas as pd

from curadoria.cluster import cluster_candidates, priority_order


def _inventory():
    return pd.DataFrame({
        "relative_path": ["a.jpg", "b.jpg", "c.jpg", "d.jpg", "e.jpg"],
        "split": ["train", "train", "test", "test", "train"],
        "class_label": ["glioma", "glioma", "glioma", "meningioma", "notumor"],
    })


def test_transitive_grouping():
    """a-b e b-c candidatos => a, b, c no mesmo cluster (transitividade)."""
    pairs = pd.DataFrame({
        "relative_path_a": ["a.jpg", "b.jpg"],
        "relative_path_b": ["b.jpg", "c.jpg"],
    })
    df = cluster_candidates(pairs, _inventory())
    clusters = df.set_index("relative_path")["cluster_id"]
    assert clusters["a.jpg"] == clusters["b.jpg"] == clusters["c.jpg"]
    assert "d.jpg" not in clusters.index  # não é candidato de ninguém


def test_cross_split_and_cross_class_flags():
    # a.jpg: train/glioma; d.jpg: test/meningioma -> split e classe diferentes.
    pairs = pd.DataFrame({
        "relative_path_a": ["a.jpg"],
        "relative_path_b": ["d.jpg"],
    })
    df = cluster_candidates(pairs, _inventory())
    row = df.iloc[0]
    assert bool(row["cross_split"])
    assert bool(row["cross_class"])


def test_disjoint_pairs_form_separate_clusters():
    pairs = pd.DataFrame({
        "relative_path_a": ["a.jpg", "d.jpg"],
        "relative_path_b": ["b.jpg", "e.jpg"],
    })
    df = cluster_candidates(pairs, _inventory())
    clusters = df.set_index("relative_path")["cluster_id"]
    assert clusters["a.jpg"] == clusters["b.jpg"]
    assert clusters["d.jpg"] == clusters["e.jpg"]
    assert clusters["a.jpg"] != clusters["d.jpg"]


def test_priority_order_puts_different_class_first():
    scored = pd.DataFrame({
        "relative_path_a": ["x.jpg", "y.jpg"],
        "relative_path_b": ["x2.jpg", "y2.jpg"],
        "different_class": [False, True],
        "cross_split": [True, False],
        "ssim": [0.99, 0.80],
    })
    ordered = priority_order(scored)
    assert ordered.iloc[0]["different_class"] == True  # noqa: E712
