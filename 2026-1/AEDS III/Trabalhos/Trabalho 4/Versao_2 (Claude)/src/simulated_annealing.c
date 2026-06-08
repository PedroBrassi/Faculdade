#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "simulated_annealing.h"
#include "partition_problem.h"

/* Parâmetros base */
#define BASE_INIT_TEMP     1000.0
#define BASE_COOLING_RATE  0.995
#define MIN_TEMP           0.001
#define BASE_MAX_ITER      1000

/* Parâmetros para instâncias grandes */
#define LARGE_N_THRESHOLD  10000
#define LARGE_MAX_ITER     200
#define LARGE_COOLING_RATE 0.99

/* =========================================================
   Simulated Annealing

   Melhorias aplicadas:
   1. Inicialização gulosa — parte de uma boa solução
   2. Avaliação incremental O(1) — sem recalcular tudo
   3. Parâmetros adaptativos por tamanho de instância
   4. Reverte somas corretamente ao rejeitar vizinho
========================================================= */
long long simulatedAnnealing(int values[], int n, int solution[]) {

    double cooling_rate = (n > LARGE_N_THRESHOLD) ? LARGE_COOLING_RATE : BASE_COOLING_RATE;
    int    max_iter     = (n > LARGE_N_THRESHOLD) ? LARGE_MAX_ITER     : BASE_MAX_ITER;

    if (n > LARGE_N_THRESHOLD)
        printf("[SA] Instancia grande (%d): max_iter=%d, cooling=%.3f\n",
               n, max_iter, cooling_rate);

    int *current = malloc(n * sizeof(int));
    int *best    = malloc(n * sizeof(int));
    if (!current || !best) { free(current); free(best); return (long long)1e18; }

    /* inicialização gulosa */
    greedySolution(values, current, n);

    long long sumA = 0, sumB = 0;
    for (int i = 0; i < n; i++) {
        if (current[i] == 0) sumA += values[i];
        else                  sumB += values[i];
    }
    long long currentFitness = llabs(sumA - sumB);
    long long bestFitness    = currentFitness;
    for (int i = 0; i < n; i++) best[i] = current[i];

    double temperature = BASE_INIT_TEMP;

    while (temperature > MIN_TEMP) {
        for (int iter = 0; iter < max_iter; iter++) {

            int pos       = rand() % n;
            int old_group = current[pos];

            /* atualiza somas incrementalmente O(1) */
            if (old_group == 0) { sumA -= values[pos]; sumB += values[pos]; }
            else                { sumB -= values[pos]; sumA += values[pos]; }

            long long newFitness = llabs(sumA - sumB);
            long long delta      = newFitness - currentFitness;

            if (delta < 0 || (double)rand() / RAND_MAX < exp(-delta / temperature)) {
                current[pos]   = 1 - old_group;
                currentFitness = newFitness;
                if (currentFitness < bestFitness) {
                    bestFitness = currentFitness;
                    for (int i = 0; i < n; i++) best[i] = current[i];
                }
            } else {
                /* reverte somas */
                if (old_group == 0) { sumA += values[pos]; sumB -= values[pos]; }
                else                { sumB += values[pos]; sumA -= values[pos]; }
            }
        }
        temperature *= cooling_rate;
    }

    for (int i = 0; i < n; i++) solution[i] = best[i];
    free(current);
    free(best);
    return bestFitness;
}
