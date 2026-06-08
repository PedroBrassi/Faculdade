#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "genetic_algorithm.h"
#include "partition_problem.h"

/* Parâmetros padrão */
#define DEFAULT_POP_SIZE      50
#define DEFAULT_GENERATIONS   200
#define DEFAULT_MUTATION_RATE 0.01

/* Parâmetros reduzidos para instâncias grandes */
#define LARGE_N_THRESHOLD     10000
#define LARGE_POP_SIZE        10
#define LARGE_GENERATIONS     50

typedef struct {
    char      *chromosome;
    long long  fitness;
} Individual;

/* =========================================================
   Avaliação de fitness via char*
========================================================= */
static long long evalFitness(int values[], const char *chrom, int n) {
    long long sumA = 0, sumB = 0;
    for (int i = 0; i < n; i++) {
        if (chrom[i] == 0) sumA += values[i];
        else               sumB += values[i];
    }
    return llabs(sumA - sumB);
}

/* =========================================================
   Inicialização gulosa para o melhor indivíduo da população
   e aleatória para os demais
========================================================= */
static void initPopulation(Individual pop[], int pop_size, int values[], int n) {
    for (int i = 0; i < pop_size; i++) {
        pop[i].chromosome = malloc(n * sizeof(char));
        if (!pop[i].chromosome) { fprintf(stderr, "Erro de alocacao\n"); exit(1); }

        if (i == 0) {
            /* primeiro indivíduo: inicialização gulosa */
            int *tmp = malloc(n * sizeof(int));
            if (tmp) {
                greedySolution(values, tmp, n);
                for (int j = 0; j < n; j++) pop[i].chromosome[j] = (char)tmp[j];
                free(tmp);
            } else {
                for (int j = 0; j < n; j++) pop[i].chromosome[j] = rand() % 2;
            }
        } else {
            for (int j = 0; j < n; j++) pop[i].chromosome[j] = rand() % 2;
        }

        pop[i].fitness = evalFitness(values, pop[i].chromosome, n);
    }
}

/* =========================================================
   Torneio binário
========================================================= */
static int tournament(Individual pop[], int pop_size) {
    int a = rand() % pop_size;
    int b = rand() % pop_size;
    return (pop[a].fitness < pop[b].fitness) ? a : b;
}

/* =========================================================
   Crossover uniforme — cada gene herdado de um dos pais
   com prob. 0.5; preserva padrões distribuídos no vetor
========================================================= */
static void crossoverUniform(const char *p1, const char *p2, char *child, int n) {
    for (int i = 0; i < n; i++)
        child[i] = ((double)rand() / RAND_MAX < 0.5) ? p1[i] : p2[i];
}

/* =========================================================
   Mutação bit a bit
========================================================= */
static void mutate(char *chrom, int n, double rate) {
    for (int i = 0; i < n; i++)
        if ((double)rand() / RAND_MAX < rate)
            chrom[i] = 1 - chrom[i];
}

/* =========================================================
   Libera cromossomos de um array de indivíduos
========================================================= */
static void freeChromosomes(Individual pop[], int size) {
    for (int i = 0; i < size; i++) {
        free(pop[i].chromosome);
        pop[i].chromosome = NULL;
    }
}

/* =========================================================
   Índice do melhor indivíduo da população
========================================================= */
static int bestIndex(Individual pop[], int pop_size) {
    int best = 0;
    for (int i = 1; i < pop_size; i++)
        if (pop[i].fitness < pop[best].fitness) best = i;
    return best;
}

/* =========================================================
   Algoritmo Genético

   Melhorias aplicadas:
   1. Bug fix: free(pop[i].chromosome) antes de substituir
   2. Bug fix: deep copy do melhor (sem dangling pointer)
   3. Elitismo: melhor indivíduo copiado direto para new_pop[0]
   4. Crossover uniforme em vez de corte único
   5. Primeiro indivíduo inicializado com solução gulosa
========================================================= */
long long geneticAlgorithm(int values[], int n, int solution[]) {

    int    pop_size      = (n > LARGE_N_THRESHOLD) ? LARGE_POP_SIZE   : DEFAULT_POP_SIZE;
    int    generations   = (n > LARGE_N_THRESHOLD) ? LARGE_GENERATIONS : DEFAULT_GENERATIONS;
    double mutation_rate = DEFAULT_MUTATION_RATE;

    if (n > LARGE_N_THRESHOLD)
        printf("[GA] Instancia grande (%d): pop=%d, gens=%d\n", n, pop_size, generations);

    Individual *pop     = malloc(pop_size * sizeof(Individual));
    Individual *new_pop = malloc(pop_size * sizeof(Individual));
    if (!pop || !new_pop) { free(pop); free(new_pop); return (long long)1e18; }

    initPopulation(pop, pop_size, values, n);

    int bi = bestIndex(pop, pop_size);
    long long bestFitness = pop[bi].fitness;

    /* deep copy do melhor cromossomo */
    char *bestChrom = malloc(n * sizeof(char));
    if (!bestChrom) {
        freeChromosomes(pop, pop_size); free(pop); free(new_pop);
        return (long long)1e18;
    }
    memcpy(bestChrom, pop[bi].chromosome, n * sizeof(char));

    for (int g = 0; g < generations; g++) {

        /* --- Elitismo: new_pop[0] recebe o melhor atual --- */
        new_pop[0].chromosome = malloc(n * sizeof(char));
        if (!new_pop[0].chromosome) break;
        memcpy(new_pop[0].chromosome, bestChrom, n * sizeof(char));
        new_pop[0].fitness = bestFitness;

        /* --- Gera o restante da nova população --- */
        for (int i = 1; i < pop_size; i++) {
            int p1 = tournament(pop, pop_size);
            int p2 = tournament(pop, pop_size);

            new_pop[i].chromosome = malloc(n * sizeof(char));
            if (!new_pop[i].chromosome) {
                /* libera o que já foi alocado nesta geração */
                for (int k = 0; k <= i; k++) {
                    free(new_pop[k].chromosome);
                    new_pop[k].chromosome = NULL;
                }
                goto done;
            }

            crossoverUniform(pop[p1].chromosome, pop[p2].chromosome,
                             new_pop[i].chromosome, n);
            mutate(new_pop[i].chromosome, n, mutation_rate);
            new_pop[i].fitness = evalFitness(values, new_pop[i].chromosome, n);

            if (new_pop[i].fitness < bestFitness) {
                bestFitness = new_pop[i].fitness;
                memcpy(bestChrom, new_pop[i].chromosome, n * sizeof(char));
            }
        }

        /* libera cromossomos da população antiga e troca ponteiros */
        freeChromosomes(pop, pop_size);
        Individual *tmp = pop;
        pop     = new_pop;
        new_pop = tmp;
    }

done:
    for (int i = 0; i < n; i++) solution[i] = bestChrom[i];

    free(bestChrom);
    freeChromosomes(pop, pop_size);
    free(pop);
    free(new_pop);

    return bestFitness;
}
