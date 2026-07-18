#include <stdio.h>
#include <stdlib.h>
#include "../include/guloso.h"
#include "../include/partition.h"

/**
 * Estrutura auxiliar utilizada para nao perder o rastro da posicao original
 * dos elementos na instancia apos a ordenacao decrescente.
 */
typedef struct {
    long long valor;     // Guarda o valor numerico do item
    int id_original;     // Guarda a posicao/indice original do item no vetor da Instancia
} ItemGuloso;

/**
 * Funcao estatica de comparacao (callback) utilizada pelo qsort da biblioteca padrao (stdlib.h).
 * Configurada para ordenar os elementos em ORDEM DECRESCENTE (do maior para o menor).
 */
static int comparar_itens_decrescente(const void *a, const void *b) {
    // Cast dos ponteiros genericos (void*) para o tipo estruturado correto
    long long va = ((const ItemGuloso *)a)->valor;
    long long vb = ((const ItemGuloso *)b)->valor;
    
    // Retorna -1 se o primeiro for maior, 1 se o segundo for maior, ou 0 se forem iguais.
    // Isso forca o qsort a empurrar os maiores valores para o inicio do vetor.
    return (va > vb) ? -1 : (va < vb) ? 1 : 0;
}

/**
 * Execucao do Algoritmo Guloso para o Problema da Particao.
 * Implementa a heuristica classica de LPT (Longest Processing Time) adaptada.
 */
Solucao executar_algoritmo_guloso(const Instancia *inst) {
    Solucao sol;
    // Aloca memoria para o vetor de configuracao que guardara a solucao (0 para Grupo A, 1 para Grupo B)
    sol.config = (int *)malloc(inst->n * sizeof(int));
    sol.fitness = -1; // Inicializa o fitness com valor invalido ate que seja avaliado

    // --- 1. PREPARACAO E COPIA DOS DADOS ---
    // Aloca um vetor auxiliar da estrutura para podermos ordenar os itens sem alterar a struct original
    ItemGuloso *itens = (ItemGuloso *)malloc(inst->n * sizeof(ItemGuloso));
    
    // Transfere os dados da instancia para o vetor auxiliar registrando seus indices de origem
    for (int i = 0; i < inst->n; i++) {
        itens[i].valor = inst->valores[i];
        itens[i].id_original = i;
    }

    // --- 2. ORDENACAO ---
    // Ordena o vetor auxiliar utilizando o algoritmo QuickSort com complexidade O(N log N)
    qsort(itens, inst->n, sizeof(ItemGuloso), comparar_itens_decrescente);

    // Acumuladores de somas de longo alcance para evitar estouro de inteiros (overflow)
    long long soma_A = 0, soma_B = 0;

    // --- 3. DISTRIBUICAO GULOSA (PASSO A PASSO) ---
    // Percorre todos os itens já ordenados, do maior para o menor valor
    for (int i = 0; i < inst->n; i++) {
        int idx = itens[i].id_original; // Recupera onde este item ficava originalmente
        long long val = itens[i].valor;  // Valor do item atual

        // Estrategia Gulosa: Coloca o elemento no grupo que estiver mais "leve" no momento
        if (soma_A <= soma_B) {
            sol.config[idx] = 0;   // Aloca o item original no grupo A
            soma_A += val;         // Atualiza o peso total acumulado do grupo A
        } else {
            sol.config[idx] = 1;   // Aloca o item original no grupo B
            soma_B += val;         // Atualiza o peso total acumulado do grupo B
        }
    }

    // --- 4. FINALIZACAO ---
    // Chama a funcao do projeto para calcular formalmente a diferenca absoluta (fitness)
    avaliar_solucao(&sol, inst);

    // Libera a memoria alocada para o vetor auxiliar (evita memory leaks)
    free(itens);
    
    // Retorna a estrutura da solucao com o vetor preenchido e seu fitness calculado
    return sol;
}