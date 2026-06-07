#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "partition_problem.h"

/* =========================================================
   Gera uma solução aleatória

   0 -> grupo A
   1 -> grupo B
========================================================= */
void generateRandomSolution(int solution[],int n) {

    for (int i = 0; i < n; i++) {
        solution[i] = rand() % 2;
    }
}

/* =========================================================
   Calcula a qualidade da solução

   fitness = |sumA - sumB|

   Quanto menor, melhor.
========================================================= */
long long evaluateFitness(int values[],int solution[],int n) {

    long long sumA = 0;
    long long sumB = 0;

    for (int i = 0; i < n; i++) {

        if (solution[i] == 0) {
            sumA += values[i];
        }

        else {
            sumB += values[i];
        }
    }

    return llabs(sumA - sumB);
}

/* =========================================================
   Algoritmo Partition Problem

   Gera uma solução aleatória
   e retorna o fitness
========================================================= */
long long randomPartition(int values[],int n,int solution[]) {

    generateRandomSolution(solution,n);

    return evaluateFitness(values,solution,n);
}

/* =========================================================
   Exibe a partição atual
========================================================= */
void printPartition(int values[],int solution[],int n) {

    long long sumA = 0;
    long long sumB = 0;

    printf("\n===== SUBSET A =====\n");

    for (int i = 0; i < n; i++) {

        if (solution[i] == 0) {

            printf("%d ",values[i]);

            sumA += values[i];
        }
    }

    printf("\n");
    printf("Sum A = %lld\n",sumA);

    printf("\n===== SUBSET B =====\n");

    for (int i = 0; i < n; i++) {

        if (solution[i] == 1) {

            printf("%d ",values[i]);

            sumB += values[i];
        }
    }

    printf("\n");
    printf("Sum B = %lld\n",sumB);
}