#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#include "execucao.h"
#include "leitor_instancias.h"
#include "partition_problem.h"
#include "simulated_annealing.h"
#include "genetic_algorithm.h"

#define NUM_SIZES          7
#define INSTANCES_PER_SIZE 5
#define RUNS_PER_INSTANCE  5       /* execuções por instância para estatística */
#define TIMEOUT_SECONDS    30      /* tempo máximo por algoritmo/instância */

/* =========================================================
   PATH GLOBAL DO RUN
========================================================= */
static char RESULTS_PATH[256] = "Results";

void setResultsPath(const char *path) {
    if (path && strlen(path) > 0)
        snprintf(RESULTS_PATH, sizeof(RESULTS_PATH), "%s", path);
}

/* =========================================================
   CRIA PASTAS DO RUN
========================================================= */
void createResultsFolder() {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s/graficos", RESULTS_PATH);
    if (system(cmd) != 0)
        printf("Erro ao criar diretório de resultados\n");
}

/* =========================================================
   ARQUIVO GERAL
========================================================= */
FILE *openGeneralResults() {
    char path[512];
    snprintf(path, sizeof(path), "%s/TodosResultados.txt", RESULTS_PATH);

    FILE *file = fopen(path, "w");
    if (!file) { printf("Erro ao criar %s\n", path); return NULL; }

    fprintf(file, "Algoritmo,Instancia,Tamanho,Run,Qualidade,TempoExecucao\n");
    return file;
}

/* =========================================================
   ARQUIVO POR ALGORITMO
========================================================= */
FILE *openAlgorithmFile(const char *algorithmName) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s.txt", RESULTS_PATH, algorithmName);

    FILE *file = fopen(path, "w");
    if (!file) printf("Erro ao criar %s\n", path);
    return file;
}

/* =========================================================
   VERIFICA SE AS INSTÂNCIAS EXISTEM
========================================================= */
static int checkInstancesExist(int sizes[], int num_sizes) {
    char path[300];
    int missing = 0;

    for (int i = 0; i < num_sizes; i++) {
        snprintf(path, sizeof(path),
                 "instances/instancia_%d/instancia_%d_1.txt",
                 sizes[i], sizes[i]);
        FILE *f = fopen(path, "r");
        if (!f) {
            printf("[AVISO] Instancia nao encontrada: %s\n", path);
            missing++;
        } else {
            fclose(f);
        }
    }

    if (missing > 0) {
        printf("\nERRO: %d grupo(s) de instancias nao encontrados.\n", missing);
        printf("Execute primeiro: make instancias\n\n");
        return 0;
    }
    return 1;
}

/* =========================================================
   TIMEOUT via SIGALRM
========================================================= */
static volatile sig_atomic_t timed_out = 0;
static sigjmp_buf alarm_buf;

static void alarm_handler(int sig) {
    (void)sig;
    timed_out = 1;
    siglongjmp(alarm_buf, 1);
}

/* =========================================================
   TABELA RESUMO no terminal
   CORRIGIDO: agora usa NUM_SIZES em vez de valor fixo 8
========================================================= */
static void printSummaryTable(int sizes[], int num_sizes,
                               const char *alg_names[], int num_algs,
                               double avg_fitness[][NUM_SIZES]) {
    printf("\n");
    printf("========================================================\n");
    printf("  RESUMO — Fitness medio por algoritmo e tamanho\n");
    printf("========================================================\n");

    /* cabeçalho */
    printf("%-22s", "Algoritmo");
    for (int i = 0; i < num_sizes; i++) {
        char label[12];
        int s = sizes[i];
        if      (s >= 1000000) snprintf(label, sizeof(label), "%dM", s/1000000);
        else if (s >= 1000)    snprintf(label, sizeof(label), "%dk", s/1000);
        else                   snprintf(label, sizeof(label), "%d",  s);
        printf("%9s", label);
    }
    printf("\n");

    printf("%-22s", "----------------------");
    for (int i = 0; i < num_sizes; i++) printf("%9s", "---------");
    printf("\n");

    /* linhas */
    for (int a = 0; a < num_algs; a++) {
        printf("%-22s", alg_names[a]);
        for (int i = 0; i < num_sizes; i++) {
            if (avg_fitness[a][i] < 0)
                printf("%9s", "timeout");
            else if (avg_fitness[a][i] == 0)
                printf("%9.0f", avg_fitness[a][i]);
            else if (avg_fitness[a][i] < 1e6)
                printf("%9.0f", avg_fitness[a][i]);
            else
                printf("%9.2e", avg_fitness[a][i]);
        }
        printf("\n");
    }
    printf("========================================================\n\n");
}

/* =========================================================
   EXECUÇÃO PRINCIPAL
========================================================= */
void runExecutions() {

    int sizes[NUM_SIZES] = {
        100, 200, 500, 1000,
        2500, 5000, 10000      /* CORRIGIDO: último elemento era 1000, agora 10000 */
    };

    /* verifica instâncias antes de começar */
    if (!checkInstancesExist(sizes, NUM_SIZES)) return;

    createResultsFolder();

    FILE *generalResults = openGeneralResults();
    if (!generalResults) return;

    Algorithm algorithms[] = {
        {"PartitionProblem",  randomPartition},
        {"Greedy",            greedyPartition},
        {"SimulatedAnnealing", simulatedAnnealing},
        {"GeneticAlgorithm",  geneticAlgorithm}
    };
    int numAlgorithms = sizeof(algorithms) / sizeof(algorithms[0]);

    FILE *algorithmFiles[numAlgorithms];
    for (int a = 0; a < numAlgorithms; a++) {
        algorithmFiles[a] = openAlgorithmFile(algorithms[a].name);
        if (!algorithmFiles[a]) { fclose(generalResults); return; }
    }

    /* matriz para a tabela resumo: avg_fitness[alg][size] */
    double avg_fitness[numAlgorithms][NUM_SIZES];
    for (int a = 0; a < numAlgorithms; a++)
        for (int i = 0; i < NUM_SIZES; i++)
            avg_fitness[a][i] = 0.0;

    /* configura handler de timeout */
    struct sigaction sa = {0};
    sa.sa_handler = alarm_handler;
    sigaction(SIGALRM, &sa, NULL);

    for (int i = 0; i < NUM_SIZES; i++) {

        printf("\n=====================================\n");
        printf("Tamanho %d\n", sizes[i]);
        printf("=====================================\n");

        for (int a = 0; a < numAlgorithms; a++)
            fprintf(algorithmFiles[a],
                    "====================\n===== %d =====\n====================\n\n",
                    sizes[i]);

        /* acumuladores por algoritmo para este tamanho */
        double sumFitAll[numAlgorithms], sumTimeAll[numAlgorithms];
        int    countAll[numAlgorithms];
        for (int a = 0; a < numAlgorithms; a++) {
            sumFitAll[a] = sumTimeAll[a] = 0.0;
            countAll[a] = 0;
        }

        volatile int j;
        for (j = 1; j <= INSTANCES_PER_SIZE; j++) {

            char path[300];
            snprintf(path, sizeof(path),
                     "instances/instancia_%d/instancia_%d_%d.txt",
                     sizes[i], sizes[i], j);

            int  n;
            int *values = readInstance(path, &n);
            if (!values) { printf("Erro ao ler instancia %d.\n", j); continue; }

            printf("\n  Instancia %d:\n", j);

            for (int a = 0; a < numAlgorithms; a++) {

                /* acumuladores para os RUNS_PER_INSTANCE runs desta instância */
                double sumFit = 0.0, sumTime = 0.0;
                int    count  = 0;
                int    to     = 0;   /* flag timeout */

                fprintf(algorithmFiles[a], "Instancia %d:\n", j);

                for (int r = 0; r < RUNS_PER_INSTANCE; r++) {

                    int *solution = malloc(n * sizeof(int));
                    if (!solution) continue;
                    for (int k = 0; k < n; k++) solution[k] = 0;

                    timed_out = 0;
                    alarm(TIMEOUT_SECONDS);

                    long long fitness;
                    double    elapsed;

                    if (sigsetjmp(alarm_buf, 1) == 0) {
                        clock_t start = clock();
                        fitness       = algorithms[a].function(values, n, solution);
                        clock_t end   = clock();
                        alarm(0);
                        elapsed = (double)(end - start) / CLOCKS_PER_SEC;

                        sumFit  += (double)fitness;
                        sumTime += elapsed;
                        count++;

                        fprintf(algorithmFiles[a],
                                "  Run %d: Fitness %lld | Tempo %.6fs\n",
                                r + 1, fitness, elapsed);

                        fprintf(generalResults,
                                "%s,instancia_%d_%d.txt,%d,%d,%lld,%.6f\n",
                                algorithms[a].name,
                                sizes[i], j, n, r + 1,
                                fitness, elapsed);

                        printf("    [%s] run %d | Fitness %lld | %.4fs\n",
                               algorithms[a].name, r + 1, fitness, elapsed);
                    } else {
                        alarm(0);
                        to = 1;
                        fprintf(algorithmFiles[a],
                                "  Run %d: TIMEOUT (>%ds)\n", r + 1, TIMEOUT_SECONDS);
                        fprintf(generalResults,
                                "%s,instancia_%d_%d.txt,%d,%d,timeout,timeout\n",
                                algorithms[a].name,
                                sizes[i], j, n, r + 1);
                        printf("    [%s] run %d | TIMEOUT\n",
                               algorithms[a].name, r + 1);
                        free(solution);
                        break;   /* não tenta mais runs desta instância/algoritmo */
                    }

                    free(solution);
                }

                /* média dos runs desta instância */
                if (count > 0) {
                    double mFit  = sumFit  / count;
                    double mTime = sumTime / count;

                    fprintf(algorithmFiles[a],
                            "  Media (%d runs): Fitness %.2f | Tempo %.6fs\n\n",
                            count, mFit, mTime);

                    /* acumula na média do tamanho */
                    sumFitAll[a]  += mFit;
                    sumTimeAll[a] += mTime;
                    countAll[a]++;
                } else {
                    fprintf(algorithmFiles[a], "  Todas as runs: TIMEOUT\n\n");
                }
                (void)to;
            }

            free(values);
        }

        /* -------------------------------------------------
           Média geral deste tamanho (todas as instâncias)
        ------------------------------------------------- */
        printf("\n  --- Medias tamanho %d ---\n", sizes[i]);

        for (int a = 0; a < numAlgorithms; a++) {
            if (countAll[a] == 0) {
                avg_fitness[a][i] = -1.0;   /* indica timeout total */
                fprintf(algorithmFiles[a],
                        "--- Media geral: TIMEOUT ---\n\n");
                fprintf(generalResults,
                        "%s,media_%d,%d,0,timeout,timeout\n",
                        algorithms[a].name, sizes[i], sizes[i]);
                printf("  [%s] TIMEOUT\n", algorithms[a].name);
            } else {
                double mFit  = sumFitAll[a]  / countAll[a];
                double mTime = sumTimeAll[a] / countAll[a];
                avg_fitness[a][i] = mFit;

                fprintf(algorithmFiles[a],
                        "--- Media geral (%d instancias validas):\n"
                        "    Fitness medio: %.2f\n"
                        "    Tempo medio:   %.6f s\n"
                        "---\n\n",
                        countAll[a], mFit, mTime);

                fprintf(generalResults,
                        "%s,media_%d,%d,0,%.2f,%.6f\n",
                        algorithms[a].name, sizes[i], sizes[i],
                        mFit, mTime);

                printf("  [%s] Fitness medio %.2f | Tempo medio %.6fs\n",
                       algorithms[a].name, mFit, mTime);
            }
        }
    }

    for (int a = 0; a < numAlgorithms; a++)
        fclose(algorithmFiles[a]);
    fclose(generalResults);

    /* tabela resumo no terminal */
    const char *names[numAlgorithms];
    for (int a = 0; a < numAlgorithms; a++) names[a] = algorithms[a].name;
    printSummaryTable(sizes, NUM_SIZES, names, numAlgorithms, avg_fitness);

    printf("Todos os resultados salvos em %s\n", RESULTS_PATH);
}