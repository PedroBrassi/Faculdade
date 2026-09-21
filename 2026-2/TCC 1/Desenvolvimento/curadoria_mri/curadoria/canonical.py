"""
Canonicalização diédrica e hashes.

Ideia central: duas imagens que representam o mesmo conteúdo mas diferem
por rotação (0/90/180/270) ou reflexão (horizontal/vertical) devem
produzir a MESMA assinatura canônica.

Implementação: para cada imagem, geramos as 8 transformações do grupo
diédrico D4 e escolhemos como assinatura o menor hash entre elas.

Propriedade garantida:
    Se B = g(A) para algum g em D4, então
    canonical_content_id(A) == canonical_content_id(B).
"""
from __future__ import annotations

import hashlib
from dataclasses import dataclass
from pathlib import Path

import imagehash
import numpy as np
from PIL import Image


def dihedral_variants(array: np.ndarray) -> list[tuple[str, np.ndarray]]:
    """As 8 transformações do grupo diédrico D4 aplicadas a um array.

    Ordem: identidade, 3 rotações, flip horizontal, e 3 compostas.
    Convenção: rotações são anti-horárias (np.rot90), reflexão é sobre
    o eixo vertical (np.fliplr).
    """
    return [
        ("identity",      array),
        ("rot90",         np.rot90(array, 1)),
        ("rot180",        np.rot90(array, 2)),
        ("rot270",        np.rot90(array, 3)),
        ("flip_h",        np.fliplr(array)),
        ("flip_h_rot90",  np.fliplr(np.rot90(array, 1))),
        ("flip_v",        np.flipud(array)),
        ("flip_h_rot270", np.fliplr(np.rot90(array, 3))),
    ]


def _array_sha256(array: np.ndarray) -> str:
    """SHA-256 do array. Inclui shape para distinguir arrays com mesma
    sequência de bytes mas formas diferentes (transpostos)."""
    h = hashlib.sha256()
    h.update(str(array.shape).encode("ascii"))
    h.update(b"|")
    h.update(np.ascontiguousarray(array).tobytes())
    return h.hexdigest()


def _array_to_pil(array: np.ndarray) -> Image.Image:
    if array.ndim == 2:
        return Image.fromarray(array, mode="L")
    if array.ndim == 3 and array.shape[2] == 3:
        return Image.fromarray(array, mode="RGB")
    if array.ndim == 3 and array.shape[2] == 4:
        return Image.fromarray(array, mode="RGBA")
    raise ValueError(f"Shape não suportado: {array.shape}")


@dataclass(frozen=True)
class VariantHash:
    """Hashes de UMA variante diédrica."""
    name: str
    pixel_sha256: str
    phash: str
    dhash: str


def compute_variant_hashes(
    path: Path,
    as_gray: bool = True,
    hash_size: int = 8,
) -> dict[str, VariantHash]:
    """Computa hashes das 8 variantes diédricas de uma imagem.

    Parâmetros
    ----------
    path : Path
        Arquivo de imagem.
    as_gray : bool
        Se True (padrão), converte para L antes de hashear. É a escolha
        correta para MRI: o conteúdo é grayscale mesmo quando armazenado
        como RGB.
    hash_size : int
        Lado do hash perceptual. 8 produz hash de 64 bits.
    """
    with Image.open(path) as img:
        mode = "L" if as_gray else "RGB"
        array = np.array(img.convert(mode), dtype=np.uint8)

    result: dict[str, VariantHash] = {}
    for name, variant in dihedral_variants(array):
        v = np.ascontiguousarray(variant)
        pil = _array_to_pil(v)
        result[name] = VariantHash(
            name=name,
            pixel_sha256=_array_sha256(v),
            phash=str(imagehash.phash(pil, hash_size=hash_size)),
            dhash=str(imagehash.dhash(pil, hash_size=hash_size)),
        )
    return result


def canonical_content_id(hashes: dict[str, VariantHash]) -> str:
    """Assinatura canônica de conteúdo.

    Retorna o menor pixel_sha256 entre as 8 variantes. Duas imagens têm
    o mesmo canonical_content_id se, e somente se, são o mesmo conteúdo
    sob alguma transformação diédrica.
    """
    return min(h.pixel_sha256 for h in hashes.values())


def hamming_distance_hex(a: str, b: str) -> int:
    """Distância de Hamming entre duas hashes hexadecimais de igual tamanho."""
    if len(a) != len(b):
        raise ValueError(f"Hashes de tamanhos diferentes: {len(a)} vs {len(b)}")
    return bin(int(a, 16) ^ int(b, 16)).count("1")