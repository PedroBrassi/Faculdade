#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "partition_problem.h"

/* =========================================================
   Gera solução aleatória
========================================================= */
void generateRandomSolution(int solution[], int n) {
    for (int i = 0; i < n; i++)
        solution[i] = rand() % 2;
}

/* =========================================================
   Calcula fitness completo: |sumA - sumB|
========================================================= */
long long evaluateFitness(int values[], int solution[], int n) {
    long long sumA = 0, sumB = 0;
    for (int i = 0; i < n; i++) {
        if (solution[i] == 0) sumA += values[i];
        else                   sumB += values[i];
    }
    return llabs(sumA - sumB);
}

/* =========================================================
   Solução inicial gulosa (greedy)

   Ordena índices por valor decrescente e vai adicionando
   cada elemento ao subconjunto com menor soma atual.
   Produz uma solução muito melhor que a aleatória.
========================================================= */

void greedySolution(int values[], int solution[], int n) {
    /* cria array de índices ordenado por valor decrescente */
    int *idx = malloc(n * sizeof(int));
    if (!idx) { generateRandomSolution(solution, n); return; }

    /* array temporário de pares (valor, índice original) para ordenar */
    int (*pairs)[2] = malloc(n * sizeof(*pairs));
    if (!pairs) { free(idx); generateRandomSolution(solution, n); return; }

    for (int i = 0; i < n; i++) { pairs[i][0] = values[i]; pairs[i][1] = i; }

    /* ordenação por valor decrescente usando comparação de pares */
    int pair_cmp(const void *a, const void *b) {
        return ((int(*)[2])b)[0][0] - ((int(*)[2])a)[0][0];
    }
    qsort(pairs, n, sizeof(*pairs), pair_cmp);

    long long sumA = 0, sumB = 0;
    for (int k = 0; k < n; k++) {
        int orig = pairs[k][1];
        if (sumA <= sumB) { solution[orig] = 0; sumA += values[orig]; }
        else              { solution[orig] = 1; sumB += values[orig]; }
    }

    free(pairs);
    free(idx);
}

/* =========================================================
   randomPartition — baseline aleatório
========================================================= */
long long randomPartition(int values[], int n, int solution[]) {
    generateRandomSolution(solution, n);
    return evaluateFitness(values, solution, n);
}

/* =========================================================
   greedyPartition — baseline guloso (exposto como algoritmo)
========================================================= */
long long greedyPartition(int values[], int n, int solution[]) {
    greedySolution(values, solution, n);
    return evaluateFitness(values, solution, n);
}

/* =========================================================
   Exibe a partição atual
========================================================= */
void printPartition(int values[], int solution[], int n) {
    long long sumA = 0, sumB = 0;
    printf("\n===== SUBSET A =====\n");
    for (int i = 0; i < n; i++) {
        if (solution[i] == 0) { printf("%d ", values[i]); sumA += values[i]; }
    }
    printf("\nSum A = %lld\n\n===== SUBSET B =====\n", sumA);
    for (int i = 0; i < n; i++) {
        if (solution[i] == 1) { printf("%d ", values[i]); sumB += values[i]; }
    }
    printf("\nSum B = %lld\n", sumB);
}
