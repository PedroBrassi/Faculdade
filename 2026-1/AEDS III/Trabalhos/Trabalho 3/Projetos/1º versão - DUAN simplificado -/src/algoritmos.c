#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "algoritmos.h"

// Algoritmo de Dijkstra usando matriz de adjacência
// Calcula o menor caminho da origem até todos os outros nós
double dijkstra(int **matriz, int num_nos, int origem) {

    // Vetor de distâncias mínimas conhecidas a partir da origem
    double *dist = (double*) malloc(num_nos * sizeof(double));

    // Marca quais nós já tiveram sua menor distância final definida
    bool *visitado = (bool*) calloc(num_nos, sizeof(bool));

    // Inicializa todas as distâncias como "infinito"
    // INT_MAX/2 evita problemas de overflow em somas
    for (int i = 0; i < num_nos; i++)
        dist[i] = INT_MAX / 2;

    // A distância da origem para ela mesma é sempre zero
    dist[origem] = 0;

    // Executa o processo principal V-1 vezes (V = número de nós)
    for (int i = 0; i < num_nos - 1; i++) {

        int u = -1;                 // próximo nó a ser processado
        int menor = INT_MAX / 2;    // menor distância encontrada

        // Escolhe o vértice não visitado com menor distância atual
        for (int j = 0; j < num_nos; j++) {
            if (!visitado[j] && dist[j] < menor) {
                menor = dist[j];
                u = j;
            }
        }

        // Se não encontrou nenhum nó válido, encerra o algoritmo
        if (u == -1) break;

        // Marca o nó como "definitivo" (menor caminho já conhecido)
        visitado[u] = true;

        // Relaxamento das arestas do nó atual
        for (int v = 0; v < num_nos; v++) {

            // Verifica se existe aresta e se o nó ainda não foi processado
            if (matriz[u][v] != 0 && !visitado[v]) {

                // Evita overflow e testa se encontrou um caminho melhor
                if (dist[u] != INT_MAX / 2 && dist[u] + matriz[u][v] < dist[v]) {

                    // Atualiza a menor distância até o nó v
                    dist[v] = dist[u] + matriz[u][v];
                }
            }
        }
    }

    // Soma das menores distâncias encontradas (métrica do projeto)
    double custo = 0;

    for (int i = 0; i < num_nos; i++) {
        if (dist[i] != INT_MAX / 2)
            custo += dist[i];
    }

    // Libera memória alocada dinamicamente
    free(dist);
    free(visitado);

    // Retorna o custo total calculado
    return custo;
}

/*
 * Implementação do Algoritmo de Duan para Single-Source Shortest Path (SSSP)
 *
 * Esta versão segue uma abordagem recursiva baseada no procedimento
 * BMSSP (Bounded Multi-Source Shortest Path), utilizando expansão local,
 * seleção de pivôs e divisão recursiva da fronteira de vértices.
 *
 * O algoritmo opera restringindo o processamento a regiões locais do
 * grafo, reduzindo o custo computacional por meio de decomposição
 * hierárquica do problema de caminhos mínimos.
 */

/* --------------------------------------------------------------------------
 * Estrutura de conjunto dinâmico de vértices.
 *
 * Utilizada para representar subconjuntos do grafo durante a execução,
 * como fronteiras locais, regiões expandidas e conjuntos de pivôs.
 * -------------------------------------------------------------------------- */
typedef struct {
    int *vertices;
    int tamanho;
    int capacidade;
} Conjunto;

/* --------------------------------------------------------------------------
 * Inicializa um conjunto dinâmico.
 *
 * Reserva memória inicial para armazenamento dos vértices.
 * -------------------------------------------------------------------------- */
void conjunto_init(Conjunto *c, int capacidade) {

    c->vertices =
        (int*) malloc(capacidade * sizeof(int));

    c->tamanho = 0;
    c->capacidade = capacidade;
}

/* --------------------------------------------------------------------------
 * Adiciona um vértice ao conjunto.
 *
 * Caso a capacidade seja excedida, o vetor é redimensionado
 * dinamicamente.
 * -------------------------------------------------------------------------- */
void conjunto_adicionar(Conjunto *c, int v) {

    if (c->tamanho >= c->capacidade) {

        c->capacidade *= 2;

        c->vertices =
            (int*) realloc(
                c->vertices,
                c->capacidade * sizeof(int)
            );
    }

    c->vertices[c->tamanho++] = v;
}

/* --------------------------------------------------------------------------
 * Libera memória associada ao conjunto dinâmico.
 * -------------------------------------------------------------------------- */
void conjunto_liberar(Conjunto *c) {

    free(c->vertices);
    c->tamanho = 0;
}

/* --------------------------------------------------------------------------
 * Constrói uma floresta de caminhos mínimos.
 *
 * Determina, para cada vértice alcançado, um predecessor compatível
 * com as distâncias atualmente calculadas.
 *
 * O resultado é uma estrutura de predecessores utilizada na seleção
 * dos pivôs.
 * -------------------------------------------------------------------------- */
void construir_floresta(
    int **matriz,
    int num_nos,
    double *dist,
    Conjunto *W,
    int *pai
) {

    /* Inicializa predecessores */
    for (int i = 0; i < num_nos; i++) {
        pai[i] = -1;
    }

    /* Determina predecessores válidos */
    for (int i = 0; i < W->tamanho; i++) {

        int v = W->vertices[i];

        if (dist[v] >= (double)(INT_MAX / 2))
            continue;

        for (int u = 0; u < num_nos; u++) {

            if (matriz[u][v] > 0) {

                double caminho =
                    dist[u] + matriz[u][v];

                if (fabs(caminho - dist[v]) < 1e-9) {
                    pai[v] = u;
                    break;
                }
            }
        }
    }
}

/* --------------------------------------------------------------------------
 * Calcula recursivamente o tamanho de uma subárvore.
 *
 * Percorre a floresta de predecessores contabilizando quantos
 * vértices pertencem ao subgrafo enraizado em u.
 *
 * Essa informação é utilizada como critério de seleção de pivôs.
 * -------------------------------------------------------------------------- */
int tamanho_subarvore(
    int u,
    int *pai,
    bool *visitado
) {

    if (visitado[u])
        return 0;

    visitado[u] = true;

    int tamanho = 1;

    for (int v = 0; v < 50000; v++) {

        if (pai[v] == u) {

            tamanho +=
                tamanho_subarvore(
                    v,
                    pai,
                    visitado
                );
        }
    }

    return tamanho;
}

/* --------------------------------------------------------------------------
 * Procedimento recursivo BMSSP
 * (Bounded Multi-Source Shortest Path)
 *
 * Processa recursivamente subconjuntos locais do grafo, limitando
 * a expansão a uma região controlada pelo parâmetro B.
 *
 * Parâmetros:
 *
 * nivel       : profundidade atual da recursão
 * B            : limite máximo de distância permitido
 * S            : fronteira atual de vértices
 * k            : profundidade da expansão local
 * t            : raio utilizado na definição das regiões locais
 * -------------------------------------------------------------------------- */
void bmssp(
    int **matriz,
    int num_nos,
    double *dist,
    bool *finalizado,
    int nivel,
    double B,
    Conjunto *S,
    int k,
    double t
) {

    int max_nivel =
        (int) pow(log(num_nos), 1.0 / 3.0);

    if (max_nivel < 1)
        max_nivel = 1;

    /* ----------------------------------------------------------------------
     * Caso base da recursão
     *
     * Quando a profundidade máxima é atingida ou o subconjunto
     * torna-se pequeno, executa um processamento local semelhante
     * ao algoritmo de Dijkstra.
     * ---------------------------------------------------------------------- */
    if (nivel >= max_nivel || S->tamanho <= 1) {

        bool *visitado_local =
            (bool*) calloc(num_nos, sizeof(bool));

        double *dist_local =
            (double*) malloc(num_nos * sizeof(double));

        for (int i = 0; i < num_nos; i++) {
            dist_local[i] = dist[i];
        }

        /* Processa vértices por menor distância */
        while (1) {

            int u = -1;

            double min_dist =
                (double)(INT_MAX / 2);

            for (int i = 0; i < S->tamanho; i++) {

                int v = S->vertices[i];

                if (!visitado_local[v] &&
                    dist_local[v] < min_dist) {

                    min_dist = dist_local[v];
                    u = v;
                }
            }

            if (u == -1)
                break;

            visitado_local[u] = true;

            for (int v = 0; v < num_nos; v++) {

                if (matriz[u][v] > 0 &&
                    !finalizado[v]) {

                    double nova =
                        dist_local[u] +
                        matriz[u][v];

                    if (nova < dist_local[v]) {

                        dist_local[v] = nova;

                        if (nova < dist[v])
                            dist[v] = nova;
                    }
                }
            }
        }

        free(visitado_local);
        free(dist_local);

        return;
    }

    /* ----------------------------------------------------------------------
     * Fase 1 — Expansão local da fronteira
     *
     * Realiza até k iterações de relaxamento restrito,
     * expandindo progressivamente o conjunto W.
     *
     * Apenas vértices cuja distância permaneça dentro
     * do limite B são considerados.
     * ---------------------------------------------------------------------- */
    Conjunto W;
    conjunto_init(&W, num_nos);

    for (int i = 0; i < S->tamanho; i++) {
        conjunto_adicionar(&W, S->vertices[i]);
    }

    bool *em_W =
        (bool*) calloc(num_nos, sizeof(bool));

    for (int i = 0; i < W.tamanho; i++) {
        em_W[W.vertices[i]] = true;
    }

    Conjunto W_prev;
    conjunto_init(&W_prev, num_nos);

    for (int i = 0; i < W.tamanho; i++) {
        conjunto_adicionar(
            &W_prev,
            W.vertices[i]
        );
    }

    for (int iter = 0; iter < k; iter++) {

        Conjunto W_next;
        conjunto_init(&W_next, num_nos);

        /* Relaxamento local */
        for (int i = 0; i < W_prev.tamanho; i++) {

            int u = W_prev.vertices[i];

            if (dist[u] >=
                (double)(INT_MAX / 2))
                continue;

            for (int v = 0; v < num_nos; v++) {

                if (matriz[u][v] == 0)
                    continue;

                if (finalizado[v])
                    continue;

                double nova_dist =
                    dist[u] + matriz[u][v];

                if (nova_dist < dist[v] &&
                    nova_dist < B) {

                    dist[v] = nova_dist;

                    if (!em_W[v]) {

                        em_W[v] = true;

                        conjunto_adicionar(
                            &W_next,
                            v
                        );
                    }
                }
            }
        }

        /* Incorpora novos vértices */
        for (int i = 0; i < W_next.tamanho; i++) {

            conjunto_adicionar(
                &W,
                W_next.vertices[i]
            );
        }

        /* Evita expansão excessiva */
        if (W.tamanho >
            k * S->tamanho) {

            conjunto_liberar(&W_next);
            conjunto_liberar(&W_prev);

            break;
        }

        conjunto_liberar(&W_prev);
        W_prev = W_next;
    }

    /* ----------------------------------------------------------------------
     * Fase 2 — Seleção de pivôs
     *
     * Seleciona vértices cuja subárvore na floresta de caminhos
     * mínimos possua tamanho suficiente.
     * ---------------------------------------------------------------------- */
    Conjunto P;
    conjunto_init(&P, num_nos);

    int *pai =
        (int*) malloc(num_nos * sizeof(int));

    construir_floresta(
        matriz,
        num_nos,
        dist,
        &W,
        pai
    );

    bool *visitado =
        (bool*) calloc(num_nos, sizeof(bool));

    for (int i = 0; i < S->tamanho; i++) {

        int u = S->vertices[i];

        if (dist[u] >=
            (double)(INT_MAX / 2))
            continue;

        memset(
            visitado,
            0,
            num_nos * sizeof(bool)
        );

        int tam_sub =
            tamanho_subarvore(
                u,
                pai,
                visitado
            );

        if (tam_sub >= k) {

            conjunto_adicionar(
                &P,
                u
            );
        }
    }

    /* Garante ao menos um pivô */
    if (P.tamanho == 0 &&
        S->tamanho > 0) {

        conjunto_adicionar(
            &P,
            S->vertices[0]
        );
    }

    /* ----------------------------------------------------------------------
     * Fase 3 — Decomposição recursiva
     *
     * Cada pivô define uma região local de influência.
     * Os vértices são agrupados em subconjuntos Sp,
     * processados recursivamente.
     * ---------------------------------------------------------------------- */
    for (int p_idx = 0;
         p_idx < P.tamanho;
         p_idx++) {

        int p = P.vertices[p_idx];

        if (dist[p] >=
            (double)(INT_MAX / 2))
            continue;

        Conjunto Sp;
        conjunto_init(&Sp, num_nos);

        for (int i = 0; i < W.tamanho; i++) {

            int v = W.vertices[i];

            if (dist[v] >=
                (double)(INT_MAX / 2))
                continue;

            if (dist[v] <= dist[p] + t) {

                bool mais_proximo = true;

                for (int q_idx = 0;
                     q_idx < P.tamanho;
                     q_idx++) {

                    if (q_idx == p_idx)
                        continue;

                    int q =
                        P.vertices[q_idx];

                    if (dist[q] >=
                        (double)(INT_MAX / 2))
                        continue;

                    if (dist[v] <= dist[q] + t &&
                        fabs(dist[v] - dist[q]) <
                        fabs(dist[v] - dist[p])) {

                        mais_proximo = false;
                        break;
                    }
                }

                if (mais_proximo) {

                    conjunto_adicionar(
                        &Sp,
                        v
                    );
                }
            }
        }

        /* Chamada recursiva */
        if (Sp.tamanho > 0) {

            bmssp(
                matriz,
                num_nos,
                dist,
                finalizado,
                nivel + 1,
                dist[p] + t,
                &Sp,
                k,
                t
            );
        }

        conjunto_liberar(&Sp);
    }

    /* Liberação de memória */
    free(pai);
    free(visitado);
    free(em_W);

    conjunto_liberar(&W);
    conjunto_liberar(&W_prev);
    conjunto_liberar(&P);
}

/* --------------------------------------------------------------------------
 * Interface pública do algoritmo de Duan
 *
 * Inicializa parâmetros, estruturas auxiliares e dispara
 * a execução do procedimento BMSSP.
 * -------------------------------------------------------------------------- */
double duan(int **matriz, int num_nos, int origem) {

    int k =
        (int) pow(log(num_nos), 1.0 / 3.0);

    if (k < 1)
        k = 1;

    double t =
        pow(log(num_nos), 2.0 / 3.0);

    if (t < 1.0)
        t = 1.0;

    double *dist =
        (double *) malloc(
            num_nos * sizeof(double)
        );

    bool *finalizado =
        (bool *) calloc(
            num_nos,
            sizeof(bool)
        );

    for (int i = 0; i < num_nos; i++) {
        dist[i] =
            (double)(INT_MAX / 2);
    }

    dist[origem] = 0.0;

    Conjunto S;
    conjunto_init(&S, num_nos);

    conjunto_adicionar(
        &S,
        origem
    );

    /* Inicia processamento recursivo */
    bmssp(
        matriz,
        num_nos,
        dist,
        finalizado,
        0,
        (double)(INT_MAX / 2),
        &S,
        k,
        t
    );

    /* Soma das menores distâncias */
    double custo = 0.0;

    for (int i = 0; i < num_nos; i++) {

        if (dist[i] <
            (double)(INT_MAX / 2)) {

            custo += dist[i];
        }
    }

    free(dist);
    free(finalizado);
    conjunto_liberar(&S);

    return custo;
}

// Implementação do Bellman-Ford usando matriz de adjacência
double bellmanFord(int **matriz, int num_nos, int origem) {

    // Vetor de distâncias: armazena o menor custo da origem até cada vértice
    double *dist = (double*) malloc(num_nos * sizeof(double));

    // Inicialização: todas as distâncias começam como "infinito"
    for (int i = 0; i < num_nos; i++)
        dist[i] = INT_MAX / 2;

    // A distância da origem para ela mesma é sempre zero
    dist[origem] = 0;

    // Laço principal do Bellman-Ford (V - 1 iterações)
    // Garante propagação correta dos menores caminhos
    for (int k = 0; k < num_nos - 1; k++) {

        bool mudou = false;
        
        // Percorre todos os possíveis vértices de origem
        for (int v = 0; v < num_nos; v++) {

            if (dist[v] == INT_MAX / 2) continue;

            // Percorre todos os possíveis vértices de destino
            for (int u = 0; u < num_nos; u++) {

                // Verifica se existe aresta v → u
                int peso = matriz[v][u];
                if (peso == 0) continue;

                // Relaxamento da aresta:
                // tenta melhorar a distância até u passando por v
                if (dist[v] + peso < dist[u]) {
                    dist[u] = dist[v] + peso;
                    mudou = true;
                }
            }
        }

        if (!mudou) break; //corta execuções inúteis
    }

    // Variável que acumula o custo total a partir da origem
    double custo = 0;

    // Soma todas as menores distâncias válidas encontradas
    for (int i = 0; i < num_nos; i++) {

        // Ignora vértices inalcançáveis (infinito)
        if (dist[i] != INT_MAX / 2) {
            custo += dist[i];
        }
    }

    // Libera memória alocada dinamicamente
    free(dist);

    // Retorna o custo total calculado

    return custo;
}