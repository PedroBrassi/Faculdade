#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>

#include "execucao.h"
#include "leitor_instancias.h"
#include "partition_problem.h"
#include "simulated_annealing.h"
#include "genetic_algorithm.h"

#define NUM_SIZES 8
#define INSTANCES_PER_SIZE 10

/* =========================================================
   PATH GLOBAL DO RUN
========================================================= */
static char RESULTS_PATH[256] = "Results";

/* =========================================================
   DEFINE PATH DO RUN
========================================================= */
void setResultsPath(const char *path) {

    if (path == NULL || strlen(path) == 0)
        return;

    snprintf(RESULTS_PATH, sizeof(RESULTS_PATH), "%s", path);
}

/* =========================================================
   CRIA PASTAS DO RUN
========================================================= */
void createResultsFolder() {

    char cmd[512];

    snprintf(cmd, sizeof(cmd),"mkdir -p %s/graficos",RESULTS_PATH);

    if (system(cmd) != 0) {
        printf("Erro ao criar diretório de resultados\n");
    }
}

/* =========================================================
   ARQUIVO GERAL
========================================================= */
FILE *openGeneralResults() {

    char path[512];

    snprintf(path, sizeof(path),
             "%s/TodosResultados.txt",
             RESULTS_PATH);

    FILE *file = fopen(path, "w");

    if (!file) {
        printf("Erro ao criar %s\n", path);
        return NULL;
    }

    fprintf(file,
            "Algoritmo,Instancia,Tamanho,Qualidade,TempoExecucao\n");

    return file;
}

/* =========================================================
   ARQUIVO POR ALGORITMO
========================================================= */
FILE *openAlgorithmFile(const char *algorithmName) {

    char path[512];

    snprintf(path, sizeof(path),
             "%s/%s.txt",
             RESULTS_PATH,
             algorithmName);

    FILE *file = fopen(path, "w");

    if (!file) {
        printf("Erro ao criar %s\n", path);
        return NULL;
    }

    return file;
}

/* =========================================================
   EXECUÇÃO PRINCIPAL
========================================================= */
void runExecutions() {

    int sizes[NUM_SIZES] = {
        100, 500, 1000, 5000,
        10000, 100000, 500000, 1000000
    };

    createResultsFolder();

    FILE *generalResults = openGeneralResults();
    if (!generalResults) return;

    Algorithm algorithms[] = {
        {"PartitionProblem", randomPartition},
        {"SimulatedAnnealing", simulatedAnnealing},
        {"GeneticAlgorithm", geneticAlgorithm}
    };

    int numberAlgorithms = sizeof(algorithms) / sizeof(algorithms[0]);

    FILE *algorithmFiles[numberAlgorithms];

    for (int a = 0; a < numberAlgorithms; a++) {

        algorithmFiles[a] =
            openAlgorithmFile(algorithms[a].name);

        if (!algorithmFiles[a]) {
            fclose(generalResults);
            return;
        }
    }

    for (int i = 0; i < NUM_SIZES; i++) {

        printf("\n=====================================\n");
        printf("Iniciando %d instancias\n", sizes[i]);
        printf("=====================================\n");

        for (int a = 0; a < numberAlgorithms; a++) {

            fprintf(algorithmFiles[a],
                    "====================\n"
                    "===== %d =====\n"
                    "====================\n\n",
                    sizes[i]);
        }

        for (int j = 1; j <= INSTANCES_PER_SIZE; j++) {

            printf("\nInstancia %d:\n", j);

            char path[300];

            snprintf(path, sizeof(path),
                     "instances/instancia_%d/instancia_%d_%d.txt",
                     sizes[i], sizes[i], j);

            int n;
            int *values = readInstance(path, &n);

            if (!values) {
                printf("Erro ao ler instancia.\n");
                continue;
            }

            for (int a = 0; a < numberAlgorithms; a++) {

                int *solution = malloc(n * sizeof(int));

                if (!solution) {
                    free(values);
                    continue;
                }

                for (int k = 0; k < n; k++)
                    solution[k] = 0;

                clock_t start = clock();

                long long fitness = algorithms[a].function(values, n, solution);

                clock_t end = clock();

                double time = (double)(end - start) / CLOCKS_PER_SEC;

                fprintf(algorithmFiles[a],
                        "Instancia %d:\n"
                        "Tempo de execução: %.6f s\n"
                        "Fitness: %lld\n\n",
                        j, time, fitness);

                fprintf(generalResults,
                        "%s,instancia_%d_%d.txt,%d,%lld,%.6f\n",
                        algorithms[a].name,
                        sizes[i], j, n,
                        fitness, time);

                printf("- Finalizada %s | Fitness %lld | %.6fs\n",
                       algorithms[a].name,
                       fitness,
                       time);

                free(solution);
            }

            printf("-- Salvo em %s\n", RESULTS_PATH);
            free(values);
        }
    }

    for (int a = 0; a < numberAlgorithms; a++)
        fclose(algorithmFiles[a]);

    fclose(generalResults);

    printf("\nTodos os resultados salvos em %s\n",
           RESULTS_PATH);
}