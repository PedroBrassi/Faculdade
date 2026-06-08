#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/simulatedAnnealing.h"
#include "../include/partition.h"

// --- PARÂMETROS GLOBAIS DO SIMULATED ANNEALING ---
#define T_INICIAL     1000.0
#define T_FINAL       0.001
#define ALFA          0.97
#define MAX_ITER_BASE 500

/**
 * Funcao auxiliar para copiar o conteudo de uma struct Solucao para outra.
 * Copia o valor do fitness e clona o vetor de configuracao (subconjuntos).
 */
void copiar_solucao(Solucao *destino, const Solucao *origem, int n) {
    destino->fitness = origem->fitness; // Copia a qualidade obtida

    for (int i = 0; i < n; i++) {
        destino->config[i] = origem->config[i]; // Copia a alocacao elemento a elemento
    }
}

/**
 * Execucao principal da Meta-heuristica Simulated Annealing (SA)
 */
Solucao executar_simulated_annealing(const Instancia *inst) {

    // --- 1. CONFIGURAÇÃO DOS PARÂMETROS DA TÊMPERA ---
    double T_inicial = T_INICIAL; // Temperatura inicial (permite alta exploração no começo)
    double T_final   = T_FINAL;   // Temperatura de congelamento (critério de parada)
    double alfa      = ALFA;      // Fator de resfriamento geométrico (taxa de decaimento de T)

    // [MELHORIA/ESCALABILIDADE] Ajusta dinamicamente as iterações internas por temperatura.
    // Garante que instâncias grandes tenham tempo de estabilização térmica suficiente.
    int max_iter_por_temp =
        (inst->n / 10 > MAX_ITER_BASE)
        ? inst->n / 10
        : MAX_ITER_BASE;

    // Estruturas para manter o controle da solução atual sob teste e o recorde histórico
    Solucao sol_atual, melhor_sol;

    sol_atual.config  = (int *)malloc(inst->n * sizeof(int));
    melhor_sol.config = (int *)malloc(inst->n * sizeof(int));

    // Verifica se houve falha de alocação de memória
    if (sol_atual.config == NULL || melhor_sol.config == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para as solucoes.\n");
        exit(EXIT_FAILURE);
    }

    // Pré-aloca um vetor de índices para o embaralhamento e varredura de vizinhança
    int *indices = (int *)malloc(inst->n * sizeof(int));

    // Verifica se houve falha de alocação de memória
    if (indices == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para os indices.\n");

        free(sol_atual.config);
        free(melhor_sol.config);

        exit(EXIT_FAILURE);
    }

    // --- 2. GERAÇÃO DA SOLUÇÃO INICIAL ALEATÓRIA E OTIMIZAÇÃO MATEMÁTICA ---

    // [OTIMIZAÇÃO CRÍTICA] Em vez de somar os dois subconjuntos (A e B) toda vez,
    // monitoramos apenas a soma_A. Como sabemos a soma_total da instância,
    // o fitness pode ser deduzido por:
    //
    // |soma_A - soma_B| = |2 * soma_A - soma_total|
    //
    // Isso reduz o custo da avaliação de O(n) para O(1).

    long long soma_A = 0;

    for (int i = 0; i < inst->n; i++) {

        // Sorteia entre Grupo 0 (A) ou Grupo 1 (B)
        sol_atual.config[i] = rand() % 2;

        // Acumula os valores pertencentes ao Grupo A
        if (sol_atual.config[i] == 0) {
            soma_A += inst->valores[i];
        }
    }

    // Aplica a fórmula simplificada de O(1) para deduzir o fitness inicial
    sol_atual.fitness = llabs(2 * soma_A - inst->soma_total);

    // Inicializa a melhor solução histórica com a configuração gerada acima
    copiar_solucao(&melhor_sol, &sol_atual, inst->n);

    // Define a temperatura corrente com o valor de partida
    double T = T_inicial;

    // --- 3. LOOP PRINCIPAL DO RESFRIAMENTO (SISTEMA TÉRMICO) ---
    while (T > T_final) {

        // Critério de parada prematura:
        // se encontrou uma partição perfeita (fitness = 0), interrompe
        if (melhor_sol.fitness == 0) {
            break;
        }

        // --- EMBARALHAMENTO DOS ÍNDICES (Algoritmo de Fisher-Yates) ---
        //
        // Garante que a escolha dos vizinhos seja aleatória,
        // mas sem o risco de avaliar repetidamente o mesmo elemento
        // antes de considerar os demais.

        for (int i = 0; i < inst->n; i++) {
            indices[i] = i;
        }

        for (int i = inst->n - 1; i > 0; i--) {

            int j = rand() % (i + 1);

            int tmp = indices[i];
            indices[i] = indices[j];
            indices[j] = tmp;
        }

        // --- LOOP INTERNO: PESQUISA DE VIZINHANÇA EM EQUILÍBRIO TÉRMICO ---
        for (int iter = 0; iter < max_iter_por_temp; iter++) {

            // Seleciona um índice do vetor de forma cíclica
            // (reutiliza a permutação gerada)
            int idx = indices[iter % inst->n];

            long long val = (long long)inst->valores[idx];

            // [OTIMIZAÇÃO EM TEMPO CONSTANTE O(1)]
            //
            // Simula o movimento de mudar o item de grupo ("bit flip").
            //
            // Se ele estava no Grupo A, ele sai.
            // Se não estava, ele entra.

            long long nova_soma_A;

            if (sol_atual.config[idx] == 0) {
                nova_soma_A = soma_A - val; // Remove o peso de A
            } else {
                nova_soma_A = soma_A + val; // Adiciona o peso em A
            }

            // Calcula o novo fitness correspondente ao vizinho simulado
            long long novo_fitness =
                llabs(2 * nova_soma_A - inst->soma_total);

            // Calcula a variação de energia (Delta F).
            // Valores negativos significam melhoria.
            long long delta_f =
                novo_fitness - sol_atual.fitness;

            int aceitar = 0;

            // Critério de Aceitação de Metropolis
            if (delta_f < 0) {

                // Se a nova solução for melhor,
                // aceita imediatamente (descida)
                aceitar = 1;

            } else {

                // Se for pior, calcula a probabilidade
                // de aceitação baseada na Temperatura atual
                double r = (double)rand() / RAND_MAX;

                if (r < exp(-((double)delta_f) / T)) {

                    // Aceita uma solução pior para escapar
                    // de mínimos locais
                    aceitar = 1;
                }
            }

            // Se o movimento vizinho foi aceito
            // (por melhoria ou por probabilidade térmica)
            if (aceitar) {

                // Efetiva o "flip" do bit
                sol_atual.config[idx] =
                    !sol_atual.config[idx];

                // Atualiza o estado da soma real
                soma_A = nova_soma_A;

                // Atualiza o fitness da solução atual
                sol_atual.fitness =
                    novo_fitness;

                // Se a solução aceita bateu o recorde global,
                // salva-a
                if (sol_atual.fitness < melhor_sol.fitness) {

                    copiar_solucao(
                        &melhor_sol,
                        &sol_atual,
                        inst->n
                    );
                }
            }
        }

        // --- ESQUEMA DE RESFRIAMENTO ---
        // Reduz a temperatura para a próxima iteração macro
        T *= alfa;
    }

    // --- 4. LIMPEZA DE MEMÓRIA (ANTI-MEMORY LEAK) ---
    free(sol_atual.config);
    free(indices);

    // Retorna a melhor solução encontrada em todo o processo
    return melhor_sol;
}