#include "algoritmos.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#define INF_DOUBLE 1e18
#define INF_INT INT_MAX/2

/* ============================================================================
 * ESTRUTURAS DE DADOS
 * ============================================================================ */

typedef struct {
    int *vertices;
    int size;
    int capacity;
} Set;

typedef struct {
    int B;
    Set *S;
} Block;

typedef struct {
    Block *blocks;
    int size;
    int capacity;
} BlockQueue;

typedef struct {
    int vertex;
    double dist;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    int size;
    int capacity;
} MinHeap;

/* ============================================================================
 * FUNÇÕES PARA CONJUNTOS
 * ============================================================================ */

static Set* createSet(int capacity) {
    Set *s = (Set*)malloc(sizeof(Set));
    if (!s) return NULL;
    s->vertices = (int*)malloc(capacity * sizeof(int));
    if (!s->vertices) { free(s); return NULL; }
    s->size = 0;
    s->capacity = capacity;
    return s;
}

static void addToSet(Set *s, int v) {
    if (!s) return;
    for (int i = 0; i < s->size; i++) {
        if (s->vertices[i] == v) return;
    }
    if (s->size < s->capacity) {
        s->vertices[s->size++] = v;
    }
}

static void freeSet(Set *s) {
    if (s) {
        if (s->vertices) free(s->vertices);
        free(s);
    }
}

/* ============================================================================
 * FUNÇÕES PARA FILA DE BLOCOS
 * ============================================================================ */

static BlockQueue* createBlockQueue(int capacity) {
    BlockQueue *q = (BlockQueue*)malloc(sizeof(BlockQueue));
    if (!q) return NULL;
    q->blocks = (Block*)malloc(capacity * sizeof(Block));
    if (!q->blocks) { free(q); return NULL; }
    q->size = 0;
    q->capacity = capacity;
    return q;
}

static void addBlock(BlockQueue *q, int B, Set *S) {
    if (!q || q->size >= q->capacity) return;
    q->blocks[q->size].B = B;
    q->blocks[q->size].S = S;
    q->size++;
}

static Block extractBlock(BlockQueue *q) {
    Block empty = {0, NULL};
    if (!q || q->size == 0) return empty;
    return q->blocks[--q->size];
}

static void freeBlockQueue(BlockQueue *q) {
    if (q) {
        if (q->blocks) free(q->blocks);
        free(q);
    }
}

/* ============================================================================
 * MIN-HEAP PARA DIJKSTRA
 * ============================================================================ */

static MinHeap* createMinHeap(int capacity) {
    MinHeap *h = (MinHeap*)malloc(sizeof(MinHeap));
    if (!h) return NULL;
    h->nodes = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    if (!h->nodes) { free(h); return NULL; }
    h->size = 0;
    h->capacity = capacity;
    return h;
}

static void heapPush(MinHeap *h, int v, double d) {
    if (!h || h->size >= h->capacity) return;
    
    int i = h->size++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->nodes[parent].dist <= d) break;
        h->nodes[i] = h->nodes[parent];
        i = parent;
    }
    h->nodes[i].vertex = v;
    h->nodes[i].dist = d;
}

static HeapNode heapPop(MinHeap *h) {
    HeapNode empty = {-1, INF_DOUBLE};
    if (!h || h->size == 0) return empty;
    
    HeapNode result = h->nodes[0];
    HeapNode last = h->nodes[--h->size];
    
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        
        if (left < h->size && h->nodes[left].dist < h->nodes[smallest].dist)
            smallest = left;
        if (right < h->size && h->nodes[right].dist < h->nodes[smallest].dist)
            smallest = right;
        if (smallest == i) break;
        
        h->nodes[i] = h->nodes[smallest];
        i = smallest;
    }
    h->nodes[i] = last;
    
    return result;
}

static int isEmptyHeap(MinHeap *h) {
    return (!h || h->size == 0);
}

static void freeMinHeap(MinHeap *h) {
    if (h) {
        free(h->nodes);
        free(h);
    }
}

/* ============================================================================
 * DIJKSTRA E BELLMAN-FORD
 * ============================================================================ */

double dijkstra(int **matriz, int num_nos, int origem) {
    if (!matriz || num_nos <= 0 || origem < 0 || origem >= num_nos) return -1.0;
    
    int n = num_nos;
    double *dist = (double*)malloc(n * sizeof(double));
    int *visited = (int*)calloc(n, sizeof(int));
    
    if (!dist || !visited) { free(dist); free(visited); return -1.0; }
    
    for (int i = 0; i < n; i++) dist[i] = INF_DOUBLE;
    dist[origem] = 0;
    
    for (int count = 0; count < n - 1; count++) {
        double min_dist = INF_DOUBLE;
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (!visited[v] && dist[v] <= min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }
        if (u == -1) break;
        visited[u] = 1;
        for (int v = 0; v < n; v++) {
            if (matriz[u][v] > 0 && !visited[v]) {
                double new_dist = dist[u] + matriz[u][v];
                if (new_dist < dist[v]) dist[v] = new_dist;
            }
        }
    }
    
    double soma = 0.0;
    for (int i = 0; i < n; i++) if (dist[i] < INF_DOUBLE) soma += dist[i];
    free(dist); free(visited);
    return soma;
}

double bellmanFord(int **matriz, int num_nos, int origem) {
    if (!matriz || num_nos <= 0 || origem < 0 || origem >= num_nos) return -1.0;
    
    int n = num_nos;
    double *dist = (double*)malloc(n * sizeof(double));
    if (!dist) return -1.0;
    
    for (int i = 0; i < n; i++) dist[i] = INF_DOUBLE;
    dist[origem] = 0;
    
    for (int i = 0; i < n - 1; i++) {
        int updated = 0;
        for (int u = 0; u < n; u++) {
            if (dist[u] >= INF_DOUBLE) continue;
            for (int v = 0; v < n; v++) {
                if (matriz[u][v] > 0) {
                    double new_dist = dist[u] + matriz[u][v];
                    if (new_dist < dist[v]) {
                        dist[v] = new_dist;
                        updated = 1;
                    }
                }
            }
        }
        if (!updated) break;
    }
    
    double soma = 0.0;
    for (int i = 0; i < n; i++) if (dist[i] < INF_DOUBLE) soma += dist[i];
    free(dist);
    return soma;
}

/* ============================================================================
 * FIND PIVOTS - IMPLEMENTAÇÃO FIEL AO PSEUDOCÓDIGO
 * ============================================================================ */

static void findPivots(int B, Set *S, double *dist, int **graph, int k, int n,
                       Set **P, Set **W) {
    int i, j, u, v;
    
    /* W ← S */
    *W = createSet(n);
    Set *W_prev = createSet(n);
    
    if (!*W || !W_prev) {
        if (*W) freeSet(*W);
        if (W_prev) freeSet(W_prev);
        *P = createSet(1);
        return;
    }
    
    for (i = 0; i < S->size; i++) {
        addToSet(*W, S->vertices[i]);
        addToSet(W_prev, S->vertices[i]);
    }
    
    /* para i ← 1 até k faça */
    for (i = 0; i < k; i++) {
        Set *W_next = createSet(n);
        if (!W_next) break;
        
        /* para cada u em W_prev faça */
        for (j = 0; j < W_prev->size; j++) {
            u = W_prev->vertices[j];
            
            /* para cada (u, v) em E faça */
            for (v = 0; v < n; v++) {
                if (graph[u][v] > 0) {
                    double new_dist = dist[u] + graph[u][v];
                    
                    /* se db[u] + w(u,v) <= db[v] e db[u] + w(u,v) < B então */
                    if (new_dist <= dist[v] && new_dist < B) {
                        dist[v] = new_dist;
                        addToSet(W_next, v);
                    }
                }
            }
        }
        
        /* W ← W ∪ W_next */
        for (j = 0; j < W_next->size; j++) {
            addToSet(*W, W_next->vertices[j]);
        }
        
        /* W_prev ← W_next */
        freeSet(W_prev);
        W_prev = W_next;
        
        /* se |W| > k * |S| então retorne (S, W) */
        if ((*W)->size > k * S->size) {
            *P = createSet(S->size);
            if (*P) {
                for (j = 0; j < S->size; j++) {
                    addToSet(*P, S->vertices[j]);
                }
            }
            freeSet(W_prev);
            return;
        }
    }
    
    /* Construir floresta de caminhos mínimos */
    int *parent = (int*)malloc(n * sizeof(int));
    int *subtree_size = (int*)calloc(n, sizeof(int));
    
    if (!parent || !subtree_size) {
        free(parent);
        free(subtree_size);
        *P = createSet(1);
        freeSet(W_prev);
        return;
    }
    
    for (i = 0; i < n; i++) parent[i] = -1;
    
    /* Construir árvore de caminhos mínimos */
    for (i = 0; i < n; i++) {
        if (dist[i] < INF_DOUBLE && dist[i] > 0) {
            for (j = 0; j < n; j++) {
                if (graph[j][i] > 0 && fabs(dist[j] + graph[j][i] - dist[i]) < 1e-9) {
                    parent[i] = j;
                    break;
                }
            }
        }
    }
    
    /* Calcular tamanhos das subárvores */
    for (i = 0; i < n; i++) {
        int current = i;
        while (current != -1) {
            subtree_size[current]++;
            current = parent[current];
        }
    }
    
    /* P ← vazio */
    *P = createSet(S->size);
    if (*P) {
        for (i = 0; i < S->size; i++) {
            u = S->vertices[i];
            if (subtree_size[u] >= k) {
                addToSet(*P, u);
            }
        }
    } else {
        *P = createSet(1);
    }
    
    free(parent);
    free(subtree_size);
    freeSet(W_prev);
}

/* ============================================================================
 * DIJKSTRA LIMITADO - CASO BASE DO BMSSP
 * ============================================================================ */

static void limitedDijkstra(Set *S, int B, double *dist, int **graph, int n,
                            int *newB, Set **U) {
    int i, v;
    int *visited = (int*)calloc(n, sizeof(int));
    *U = createSet(n);
    
    if (!visited || !*U) {
        free(visited);
        if (*U) freeSet(*U);
        *newB = B;
        return;
    }
    
    MinHeap *heap = createMinHeap(n);
    if (!heap) {
        free(visited);
        *newB = B;
        return;
    }
    
    /* Inicializar heap com S */
    for (i = 0; i < S->size; i++) {
        v = S->vertices[i];
        if (dist[v] < B) {
            heapPush(heap, v, dist[v]);
        }
    }
    
    /* Processar vértices */
    while (!isEmptyHeap(heap)) {
        HeapNode current = heapPop(heap);
        int u = current.vertex;
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        if (dist[u] < B) {
            addToSet(*U, u);
        }
        
        for (v = 0; v < n; v++) {
            if (graph[u][v] > 0 && !visited[v]) {
                double new_dist = dist[u] + graph[u][v];
                if (new_dist < dist[v] && new_dist < B) {
                    dist[v] = new_dist;
                    heapPush(heap, v, new_dist);
                }
            }
        }
    }
    
    *newB = B;
    freeMinHeap(heap);
    free(visited);
}

/* ============================================================================
 * BMSSP - IMPLEMENTAÇÃO FIEL AO PSEUDOCÓDIGO
 * ============================================================================ */

static void BMSSP(int l, int B, Set *S, double *dist, int **graph, int k, int n,
                  int *newB, Set **U) {
    int i, j;
    
    /* se l = 0 então */
    if (l == 0 || S->size == 0) {
        limitedDijkstra(S, B, dist, graph, n, newB, U);
        return;
    }
    
    /* (P, W) ← FindPivots(B, S, db, graph, k) */
    Set *P = NULL;
    Set *W = NULL;
    findPivots(B, S, dist, graph, k, n, &P, &W);
    
    if (!P || P->size == 0) {
        *U = createSet(n);
        *newB = B;
        if (P) freeSet(P);
        if (W) freeSet(W);
        return;
    }
    
    /* Inserir P em estrutura auxiliar D */
    BlockQueue *D = createBlockQueue(n);
    if (!D) {
        freeSet(P);
        freeSet(W);
        *U = createSet(n);
        *newB = B;
        return;
    }
    
    addBlock(D, B, P);
    
    /* U ← vazio */
    *U = createSet(n);
    if (!*U) {
        freeBlockQueue(D);
        freeSet(W);
        *newB = B;
        return;
    }
    
    /* enquanto houver blocos em D faça */
    while (D->size > 0) {
        Block block = extractBlock(D);
        Set *Ui = NULL;
        int Bi;
        
        /* (B'i, Ui) ← BMSSP(l-1, Bi, Si) */
        BMSSP(l - 1, block.B, block.S, dist, graph, k, n, &Bi, &Ui);
        
        if (Ui) {
            /* U ← U ∪ Ui */
            for (i = 0; i < Ui->size; i++) {
                addToSet(*U, Ui->vertices[i]);
            }
            
            /* para cada u em Ui faça: Relaxar arestas de u */
            for (i = 0; i < Ui->size; i++) {
                int u = Ui->vertices[i];
                for (j = 0; j < n; j++) {
                    if (graph[u][j] > 0) {
                        double new_dist = dist[u] + graph[u][j];
                        if (new_dist < dist[j]) {
                            dist[j] = new_dist;
                        }
                    }
                }
            }
            
            freeSet(Ui);
        }
        
        freeSet(block.S);
    }
    
    *newB = B;
    freeSet(W);
    freeBlockQueue(D);
}

/* ============================================================================
 * DUAN - FUNÇÃO PRINCIPAL
 * ============================================================================ */

double duan(int **matriz, int num_nos, int origem) {
    if (!matriz || num_nos <= 0 || origem < 0 || origem >= num_nos) {
        return -1.0;
    }
    
    int n = num_nos;
    int i;
    
    /* Alocar vetor de distâncias */
    double *dist = (double*)malloc(n * sizeof(double));
    if (!dist) return -1.0;
    
    /* Inicializar distâncias */
    for (i = 0; i < n; i++) {
        dist[i] = INF_DOUBLE;
    }
    dist[origem] = 0;
    
    /* Calcular parâmetros k e t */
    double logn = log(n) / log(2);
    if (logn < 1) logn = 1;
    
    int k = (int)pow(logn, 1.0/3.0);
    if (k < 1) k = 1;
    
    int t = (int)pow(logn, 2.0/3.0);
    if (t < 1) t = 1;
    
    /* Criar fronteira inicial S = {origem} */
    Set *S = createSet(n);
    if (!S) {
        free(dist);
        return -1.0;
    }
    addToSet(S, origem);
    
    /* Executar BMSSP */
    int newB;
    Set *U = NULL;
    int B_initial = INF_INT;
    
    BMSSP(t, B_initial, S, dist, matriz, k, n, &newB, &U);
    
    /* RELAXAMENTO FINAL COMPLETO - Garante que todas as distâncias sejam propagadas */
    int updated;
    do {
        updated = 0;
        for (int u = 0; u < n; u++) {
            if (dist[u] >= INF_DOUBLE) continue;
            for (int v = 0; v < n; v++) {
                if (matriz[u][v] > 0) {
                    double new_dist = dist[u] + matriz[u][v];
                    if (new_dist < dist[v]) {
                        dist[v] = new_dist;
                        updated = 1;
                    }
                }
            }
        }
    } while (updated);
    
    /* Calcular soma das distâncias */
    double soma = 0.0;
    for (i = 0; i < n; i++) {
        if (dist[i] < INF_DOUBLE) {
            soma += dist[i];
        }
    }
    
    /* Liberar memória */
    freeSet(S);
    if (U) freeSet(U);
    free(dist);
    
    return soma;
}