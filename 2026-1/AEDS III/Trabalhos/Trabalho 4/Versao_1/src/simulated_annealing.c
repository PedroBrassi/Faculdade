#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "simulated_annealing.h"
#include "partition_problem.h"

/* Parâmetros base (para instâncias pequenas) */
#define BASE_INIT_TEMP      1000.0
#define BASE_COOLING_RATE   0.995
#define MIN_TEMP            0.001
#define BASE_MAX_ITER       1000

/* Ajustes para instâncias grandes (n > 10.000) */
#define LARGE_N_THRESHOLD   10000
#define LARGE_MAX_ITER      200      /* reduz drasticamente o número de iterações */
#define LARGE_COOLING_RATE  0.99     /* resfria mais rápido */


void generateNeighbor(int current[], int neighbor[], int n) {
    for (int i = 0; i < n; i++)
        neighbor[i] = current[i];
    int pos = rand() % n;
    neighbor[pos] = 1 - neighbor[pos];
}

/* =========================================================
   Simulated Annealing com avaliação incremental
   e parâmetros adaptativos.
========================================================= */
long long simulatedAnnealing(int values[], int n, int solution[]) {
    /* Ajusta parâmetros conforme o tamanho da instância */
    double init_temp = BASE_INIT_TEMP;
    double cooling_rate = BASE_COOLING_RATE;
    int max_iter = BASE_MAX_ITER;

    if (n > LARGE_N_THRESHOLD) {
        max_iter = LARGE_MAX_ITER;
        cooling_rate = LARGE_COOLING_RATE;
        printf("[SA] Instância grande (%d): max_iter=%d, cooling=%.3f\n",
               n, max_iter, cooling_rate);
    }

    double temperature = init_temp;

    /* Aloca vetores para a solução atual e a melhor solução */
    int *current = (int*)malloc(n * sizeof(int));
    int *best = (int*)malloc(n * sizeof(int));
    if (!current || !best) {
        free(current);
        free(best);
        return 1e9;
    }

    /* Gera solução inicial aleatória e calcula sumA e sumB incrementalmente */
    long long sumA = 0, sumB = 0;
    for (int i = 0; i < n; i++) {
        current[i] = rand() % 2;
        if (current[i] == 0)
            sumA += values[i];
        else
            sumB += values[i];
    }
    long long currentFitness = llabs(sumA - sumB);
    long long bestFitness = currentFitness;

    /* Copia a melhor solução */
    for (int i = 0; i < n; i++)
        best[i] = current[i];

    /* Loop de resfriamento */
    while (temperature > MIN_TEMP) {
        for (int iter = 0; iter < max_iter; iter++) {
            /* Escolhe uma posição aleatória para virar o bit */
            int pos = rand() % n;
            int old_group = current[pos];
            int new_group = 1 - old_group;

            /* Atualiza as somas incrementalmente (custo O(1)) */
            if (old_group == 0) {
                /* Elemento sai do conjunto A e vai para B */
                sumA -= values[pos];
                sumB += values[pos];
            } else {
                /* Elemento sai do conjunto B e vai para A */
                sumB -= values[pos];
                sumA += values[pos];
            }

            long long newFitness = llabs(sumA - sumB);
            long long delta = newFitness - currentFitness;

            /* Critério de aceitação de Metropolis */
            if (delta < 0) {
                /* Aceita a mudança */
                currentFitness = newFitness;
                current[pos] = new_group;
                if (currentFitness < bestFitness) {
                    bestFitness = currentFitness;
                    for (int i = 0; i < n; i++)
                        best[i] = current[i];
                }
            } else {
                double probability = exp(-delta / temperature);
                if ((double)rand() / RAND_MAX < probability) {
                    /* Aceita mesmo com piora */
                    currentFitness = newFitness;
                    current[pos] = new_group;
                } else {
                    /* Reverte a mudança nas somas */
                    if (old_group == 0) {
                        sumA += values[pos];
                        sumB -= values[pos];
                    } else {
                        sumB += values[pos];
                        sumA -= values[pos];
                    }
                    /* current[pos] permanece old_group, não precisa alterar */
                }
            }
        }
        temperature *= cooling_rate;
    }

    /* Copia a melhor solução encontrada para o vetor de saída */
    for (int i = 0; i < n; i++)
        solution[i] = best[i];

    free(current);
    free(best);

    return bestFitness;
}