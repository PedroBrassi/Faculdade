#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/partition.h"
#include "../include/leitor.h"

/**
 * Inicializa uma solucao padrao (ou ingenua) para o problema.
 * Aloca a memoria necessária e define um estado inicial deterministico:
 * todos os elementos sao jogados no mesmo grupo (Grupo A).
 */
void inicializar_solucao_padrao(Solucao *sol, const Instancia *inst) {
    // [BOA PRÁTICA] Usa calloc em vez de malloc.
    // O calloc aloca a memoria e automaticamente zera todos os bits (coloca 0 em tudo).
    // Como o Grupo A e representado por 0, todos os 'n' elementos começam alocados no Grupo A de forma limpa.
    sol->config = (int *)calloc(inst->n, sizeof(int)); 
    
    // Como todos estao no Grupo A e o Grupo B está vazio (soma zero), a diferenca inicial
    // absoluta (fitness) e exatamente igual a propria soma de todos os elementos.
    sol->fitness = inst->soma_total;                   
}

/**
 * Funcao de Avaliacao (Funcao Objetivo / Fitness)
 * Calcula a qualidade de uma determinada configuracao de divisao de grupos.
 * O objetivo do Partition Problem e minimizar essa diferenca absoluta.
 */
void avaliar_solucao(Solucao *sol, const Instancia *inst) {
    // Acumuladores de longo alcance para evitar estouro de inteiros (overflow)
    long long soma_A = 0;
    long long soma_B = 0;

    // Varre o vetor de configuracao da solucao elemento por elemento
    for (int i = 0; i < inst->n; i++) {
        if (sol->config[i] == 0) {
            soma_A += inst->valores[i]; // Se o gene for 0, o item pertence ao Grupo A
        } else {
            soma_B += inst->valores[i]; // Se o gene for 1, o item pertence ao Grupo B
        }
    }

    // O Fitness e a diferenca absoluta entre as duas somas parciais.
    // Usa-se 'llabs' (Long Long Absolute Value) para garantir o calculo correto
    // do modulo da diferenca sem riscos de bugs com valores negativos ou truncamentos.
    sol->fitness = llabs(soma_A - soma_B); 
}

/**
 * Funcao utilitaria que unifica o pipeline de teste para a solucao padrao (PartitionPuro).
 * Ela abre o arquivo, carrega os dados, calcula o pior caso e limpa a memoria,
 * retornando diretamente o fitness bruto.
 */
long long obter_fitness_da_instancia(const char *caminho_arquivo) {
    // 1. Carrega os dados numericos do arquivo de entrada utilizando o modulo Leitor
    Instancia *inst = ler_instancia(caminho_arquivo);
    if (inst == NULL) {
        return -1; // Retorna um codigo de erro caso o arquivo nao exista ou esteja corrompido
    }

    // 2. Cria e avalia a solucao ingenua de base (todos no subconjunto A)
    Solucao sol;
    inicializar_solucao_padrao(&sol, inst); // Aloca e zera o vetor de configuracao
    avaliar_solucao(&sol, inst);            // Executa a soma e extrai a diferenca absoluta

    long long fitness_calculado = sol.fitness; // Preserva o resultado antes de destruir a estrutura

    // 3. Desalocacao em cascata para evitar vazamentos de memoria (Memory Leaks)
    liberar_solucao(&sol);
    liberar_instancia(inst);

    return fitness_calculado; // Retorna o fitness do pior cenário possivel para fins comparativos
}

/**
 * Funcao de desalocacao segura (Destrutor da estrutura Solucao).
 * Libera a memoria alocada para o vetor de configuracao e anula o ponteiro
 * para evitar acessos invalidos posteriores (Dangling Pointers).
 */
void liberar_solucao(Solucao *sol) {
    if (sol != NULL) {
        if (sol->config != NULL) {
            free(sol->config);  // Libera a memoria RAM alocada para o array dinâmico
            sol->config = NULL; // Defesa de codigo: garante que o ponteiro aponte para lugar nenhum
        }
    }
}