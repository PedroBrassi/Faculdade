#ifndef ALGORITMOS_H
#define ALGORITMOS_H

/**
 * Algoritmo de Dijkstra para caminhos mínimos
 * Complexidade: O(V²)
 * 
 * @param matriz Matriz de adjacência (n x n)
 * @param num_nos Número de vértices
 * @param origem Vértice fonte
 * @return Soma das distâncias mínimas
 */
double dijkstra(int **matriz, int num_nos, int origem);

/**
 * Algoritmo de Bellman-Ford para caminhos mínimos
 * Complexidade: O(V*E)
 * 
 * @param matriz Matriz de adjacência (n x n)
 * @param num_nos Número de vértices
 * @param origem Vértice fonte
 * @return Soma das distâncias mínimas
 */
double bellmanFord(int **matriz, int num_nos, int origem);

/**
 * Algoritmo de Duan et al. (2025) para SSSP
 * Complexidade teórica: O(m · log^(2/3) n)
 * Implementação fiel ao pseudocódigo do artigo original
 */
double duan(int **matriz, int num_nos, int origem);

#endif /* ALGORITMOS_H */