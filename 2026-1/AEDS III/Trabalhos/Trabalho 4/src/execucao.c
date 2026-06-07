#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#include "execucao.h"
#include "leitor_instancias.h"
#include "partition_problem.h"

/*
#include "simulated_annealing.h"
#include "genetic_algorithm.h"
*/

#define NUM_SIZES 8
#define INSTANCES_PER_SIZE 10

/* =========================================================
   Cria pasta Results
========================================================= */
void createResultsFolder() {
    mkdir("Results",0700);
}

/* =========================================================
   Abre arquivo geral
========================================================= */
FILE *openGeneralResults() {

    FILE *file = fopen("Results/TodosResultados.txt","w");

    if (file == NULL) {
        printf("Erro ao criar TodosResultados.txt\n");
        return NULL;
    }

    fprintf(file,"Algoritmo,Instancia,Tamanho,Qualidade,TempoExecucao\n\n");

    return file;
}

/* =========================================================
   Abre arquivo do algoritmo
========================================================= */
FILE *openAlgorithmFile(const char *algorithmName) {

    char fileName[200];

    sprintf(fileName,"Results/%s.txt",algorithmName);

    FILE *file = fopen(fileName,"w");

    if (file == NULL) {

        printf("Erro ao criar %s\n",fileName);

        return NULL;
    }

    return file;
}

/* =========================================================
   Executa os algoritmos para todas as intancias propostas
========================================================= */
void runExecutions() {

    int sizes[NUM_SIZES] = {
        100,
        500,
        1000,
        5000,
        10000,
        100000,
        500000,
        1000000
    };

    createResultsFolder();

    FILE *generalResults = openGeneralResults();

    if (generalResults == NULL) {
        return;
    }

    /* Lista de algoritmos */
    Algorithm algorithms[] = {

        {
            "PartitionProblem",
            randomPartition
        }

        /*
        {
            "SimulatedAnnealing",
            simulatedAnnealing
        },

        {
            "GeneticAlgorithm",
            geneticAlgorithm
        }
        */
    };

    int numberAlgorithms = sizeof(algorithms) / sizeof(algorithms[0]);

    /* Arquivos individuais */
    FILE *algorithmFiles[numberAlgorithms];

    for (int a = 0; a < numberAlgorithms; a++) {

        algorithmFiles[a] = openAlgorithmFile(algorithms[a].name);

        if (algorithmFiles[a] == NULL) {
            fclose(generalResults);
            return;
        }
    }

    /* Percorre tamanhos */
    for (int i = 0; i < NUM_SIZES; i++) {

        printf("\n=====================================\n");
        printf("Iniciando %d instancias\n", sizes[i]);
        printf("=====================================\n");

        /* Cabeçalho dos arquivos */
        for (int a = 0; a < numberAlgorithms; a++) {

            fprintf(algorithmFiles[a], "====================\n");
            fprintf(algorithmFiles[a], "===== %d =====\n", sizes[i]);
            fprintf(algorithmFiles[a], "====================\n\n");
        }

        /* Percorre instâncias */
        for (int j = 1; j <= INSTANCES_PER_SIZE; j++) {

            printf("\nInstancia %d:\n", j);

            char path[300];

            sprintf(path,"instances/instancia_%d/instancia_%d_%d.txt",sizes[i],sizes[i],j);

            int n;

            /* Lê UMA vez */
            int *values = readInstance(path, &n);

            if (values == NULL) {

                printf("Erro ao ler instancia.\n");

                continue;
            }

            /* Executa todos algoritmos */
            for (int a = 0; a < numberAlgorithms; a++) {

                int *solution = malloc(n * sizeof(int));

                if (solution == NULL) {
                    continue;
                }

                clock_t start = clock();

                long long fitness = algorithms[a].function(values, n, solution);

                clock_t end = clock();

                double executionTime =(double)(end - start) / CLOCKS_PER_SEC;

                /* Arquivo individual */
                fprintf(algorithmFiles[a], "Instancia %d:\n", j);
                fprintf(algorithmFiles[a], "Tempo de execução: %.6f secs\n", executionTime);
                fprintf(algorithmFiles[a], "Pontos (fitness): %lld\n\n", fitness);

                /* Arquivo consolidado */
                fprintf(generalResults,
                        "%s,instancia_%d_%d.txt,%d,%lld,%.6f\n",
                        algorithms[a].name,
                        sizes[i],
                        j,
                        n,
                        fitness,
                        executionTime);

                printf("- Finalizada para %s\n", algorithms[a].name);

                free(solution);
            }

            printf("-- Salvo em Results\n");

            free(values);
        }
    }

    /* Fecha arquivos */
    for (int a = 0; a < numberAlgorithms; a++) {
        fclose(algorithmFiles[a]);
    }

    fclose(generalResults);

    printf("\nTodos os resultados foram salvos na pasta Results\n");
}