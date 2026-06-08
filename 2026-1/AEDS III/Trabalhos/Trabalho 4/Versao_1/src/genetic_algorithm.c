#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "genetic_algorithm.h"
#include "partition_problem.h"

/* Parâmetros padrão para instâncias pequenas */
#define DEFAULT_POP_SIZE      50
#define DEFAULT_GENERATIONS   200
#define DEFAULT_MUTATION_RATE 0.01

/* Limiar para considerar instância "grande" */
#define LARGE_N_THRESHOLD     10000

/* Parâmetros reduzidos para instâncias grandes */
#define LARGE_POP_SIZE        10
#define LARGE_GENERATIONS     50

/* Representação do indivíduo – cromossomo como char (1 byte) */
typedef struct {
    char *chromosome;
    long long fitness;
} Individual;

/* =========================
   Avaliação de fitness usando char*
   ========================= */
static long long evaluateFitnessChar(int values[], const char *solution, int n) {
    long long sumA = 0, sumB = 0;
    for (int i = 0; i < n; i++) {
        if (solution[i] == 0)
            sumA += values[i];
        else
            sumB += values[i];
    }
    return llabs(sumA - sumB);
}

static void evaluate(Individual *ind, int values[], int n) {
    ind->fitness = evaluateFitnessChar(values, ind->chromosome, n);
}

/* =========================
   Inicializa população (aloca e preenche)
   ========================= */
static void initPopulation(Individual pop[], int pop_size, int values[], int n) {
    for (int i = 0; i < pop_size; i++) {
        pop[i].chromosome = (char*)malloc(n * sizeof(char));
        if (!pop[i].chromosome) {
            fprintf(stderr, "Erro de alocação na população\n");
            exit(1);
        }
        for (int j = 0; j < n; j++)
            pop[i].chromosome[j] = rand() % 2;
        evaluate(&pop[i], values, n);
    }
}

/* =========================
   Torneio binário
   ========================= */
static int tournament(Individual pop[], int pop_size) {
    int a = rand() % pop_size;
    int b = rand() % pop_size;
    return (pop[a].fitness < pop[b].fitness) ? a : b;
}

/* =========================
   Crossover de um ponto (trabalha com char*)
   ========================= */
static void crossover(const char *p1, const char *p2, char *child, int n) {
    int cut = rand() % n;
    memcpy(child, p1, cut);
    memcpy(child + cut, p2 + cut, n - cut);
}

/* =========================
   Mutação bit a bit
   ========================= */
static void mutate(char *chrom, int n, double rate) {
    for (int i = 0; i < n; i++) {
        if ((double)rand() / RAND_MAX < rate)
            chrom[i] = 1 - chrom[i];
    }
}

/* =========================
   Libera cromossomos de uma população (não libera o array de Individuals)
   ========================= */
static void freeChromosomes(Individual pop[], int size) {
    for (int i = 0; i < size; i++) {
        free(pop[i].chromosome);
        pop[i].chromosome = NULL;
    }
}

/* =========================
   Algoritmo Genético principal (versão corrigida)
   ========================= */
long long geneticAlgorithm(int values[], int n, int solution[]) {
    int pop_size = DEFAULT_POP_SIZE;
    int generations = DEFAULT_GENERATIONS;
    double mutation_rate = DEFAULT_MUTATION_RATE;

    if (n > LARGE_N_THRESHOLD) {
        pop_size = LARGE_POP_SIZE;
        generations = LARGE_GENERATIONS;
        printf("[GA] Instância grande (%d): pop_size=%d, gens=%d\n",
               n, pop_size, generations);
    }

    Individual *pop = (Individual*)malloc(pop_size * sizeof(Individual));
    Individual *new_pop = (Individual*)malloc(pop_size * sizeof(Individual));
    if (!pop || !new_pop) {
        free(pop); free(new_pop);
        return 1e9;
    }

    initPopulation(pop, pop_size, values, n);

    int best_idx = 0;
    for (int i = 1; i < pop_size; i++) {
        if (pop[i].fitness < pop[best_idx].fitness)
            best_idx = i;
    }
    long long best_fitness = pop[best_idx].fitness;

    for (int g = 0; g < generations; g++) {
        // Gera nova população
        for (int i = 0; i < pop_size; i++) {
            int p1 = tournament(pop, pop_size);
            int p2 = tournament(pop, pop_size);

            new_pop[i].chromosome = (char*)malloc(n * sizeof(char));
            if (!new_pop[i].chromosome) {
                freeChromosomes(pop, pop_size);
                freeChromosomes(new_pop, i);
                free(pop); free(new_pop);
                return best_fitness;
            }

            crossover(pop[p1].chromosome, pop[p2].chromosome,
                      new_pop[i].chromosome, n);
            mutate(new_pop[i].chromosome, n, mutation_rate);
            evaluate(&new_pop[i], values, n);

            if (new_pop[i].fitness < best_fitness) {
                best_fitness = new_pop[i].fitness;
                best_idx = i;
            }
        }

        // Libera cromossomos da população antiga
        freeChromosomes(pop, pop_size);

        // Troca os ponteiros
        Individual *temp = pop;
        pop = new_pop;
        new_pop = temp;
    }

    // Agora pop tem a última população válida, new_pop tem a anterior (cromossomos já liberados)
    int final_best = 0;
    for (int i = 1; i < pop_size; i++) {
        if (pop[i].fitness < pop[final_best].fitness)
            final_best = i;
    }
    best_fitness = pop[final_best].fitness;

    for (int i = 0; i < n; i++)
        solution[i] = pop[final_best].chromosome[i];

    // Libera apenas os cromossomos da população atual e os arrays
    freeChromosomes(pop, pop_size);
    free(pop);
    free(new_pop);   // new_pop aponta para a população antiga (cromossomos já liberados)

    return best_fitness;
}