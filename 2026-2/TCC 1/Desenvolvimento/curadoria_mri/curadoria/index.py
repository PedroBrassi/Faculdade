"""Índice para busca por distância de Hamming <= T em hashes de 64 bits.

Multi-Index Hashing: dividimos os 64 bits em `tables` blocos iguais.
Se duas hashes têm distância <= T, elas concordam em pelo menos
tables - T blocos. Uso de `set` por bucket evita contagem dupla quando
a mesma imagem é adicionada com múltiplas variantes.
"""
from __future__ import annotations


class HammingIndex:
    def __init__(self, hash_bits: int = 64, tables: int = 8):
        if hash_bits % tables:
            raise ValueError("hash_bits precisa ser múltiplo de tables")
        self.tables = tables
        self.chunk_bits = hash_bits // tables
        self.mask = (1 << self.chunk_bits) - 1
        self.buckets: list[dict[int, set[int]]] = [{} for _ in range(tables)]

    def add(self, hash_hex: str, item_id: int) -> None:
        h = int(hash_hex, 16)
        for i in range(self.tables):
            chunk = (h >> (i * self.chunk_bits)) & self.mask
            self.buckets[i].setdefault(chunk, set()).add(item_id)

    def query(self, hash_hex: str, max_distance: int) -> set[int]:
        h = int(hash_hex, 16)
        counts: dict[int, int] = {}
        for i in range(self.tables):
            chunk = (h >> (i * self.chunk_bits)) & self.mask
            for item_id in self.buckets[i].get(chunk, ()):
                counts[item_id] = counts.get(item_id, 0) + 1
        min_agreements = self.tables - max_distance
        return {item_id for item_id, n in counts.items() if n >= min_agreements}