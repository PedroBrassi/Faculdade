"""Testes das propriedades algébricas da canonicalização."""
from pathlib import Path

import numpy as np
import pytest
from PIL import Image

from curadoria.canonical import (
    canonical_content_id,
    compute_variant_hashes,
    dihedral_variants,
)


@pytest.fixture
def synthetic_mri(tmp_path: Path) -> Path:
    """Cria uma imagem sintética com estrutura não-trivial (para que
    rotações sejam distinguíveis)."""
    rng = np.random.default_rng(42)
    base = np.zeros((64, 64), dtype=np.uint8)
    base[16:48, 16:48] = 128
    base[24:40, 24:40] = 255
    base += rng.integers(0, 16, size=base.shape, dtype=np.uint8)
    path = tmp_path / "img.png"
    Image.fromarray(base).save(path)
    return path


def test_dihedral_has_eight_distinct_transforms():
    array = np.arange(16).reshape(4, 4)
    names = [name for name, _ in dihedral_variants(array)]
    assert len(names) == 8
    assert len(set(names)) == 8


def test_canonical_id_is_stable_under_rotation(synthetic_mri, tmp_path):
    """A propriedade central: rotação não muda o canonical_content_id."""
    with Image.open(synthetic_mri) as img:
        base = np.array(img.convert("L"), dtype=np.uint8)

    base_id = canonical_content_id(compute_variant_hashes(synthetic_mri))

    for turns in (1, 2, 3):
        rotated = np.rot90(base, turns)
        path = tmp_path / f"rot_{turns * 90}.png"
        Image.fromarray(rotated).save(path)
        assert canonical_content_id(compute_variant_hashes(path)) == base_id


def test_canonical_id_is_stable_under_flip(synthetic_mri, tmp_path):
    with Image.open(synthetic_mri) as img:
        base = np.array(img.convert("L"), dtype=np.uint8)

    base_id = canonical_content_id(compute_variant_hashes(synthetic_mri))

    for name, transform in [("flip_h", np.fliplr), ("flip_v", np.flipud)]:
        path = tmp_path / f"{name}.png"
        Image.fromarray(transform(base)).save(path)
        assert canonical_content_id(compute_variant_hashes(path)) == base_id


def test_distinct_images_have_distinct_canonical_ids(tmp_path):
    rng = np.random.default_rng(0)
    a = rng.integers(0, 256, size=(64, 64), dtype=np.uint8)
    b = rng.integers(0, 256, size=(64, 64), dtype=np.uint8)
    pa, pb = tmp_path / "a.png", tmp_path / "b.png"
    Image.fromarray(a).save(pa)
    Image.fromarray(b).save(pb)
    assert canonical_content_id(compute_variant_hashes(pa)) != \
           canonical_content_id(compute_variant_hashes(pb))