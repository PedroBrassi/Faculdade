#ifndef PARTITION_H
#define PARTITION_H

#include "leitor.h"

// Estrutura para armazenar uma solução do Partition Problem
typedef struct {
    int *config;       // Vetor binário de tamanho 'n' (0 = Grupo A, 1 = Grupo B)
    long long fitness; // Diferença absoluta: |Soma_A - Soma_B|
} Solucao;

// Inicializa uma solução com todos os elementos em um grupo (configuração inicial padrão)
void inicializar_solucao_padrao(Solucao *sol, const Instancia *inst);

// Calcula a aptidão da solução: objetivo é chegar o mais próximo de 0
void avaliar_solucao(Solucao *sol, const Instancia *inst);

// Função que recebe o caminho de um arquivo, processa e retorna o fitness de uma configuração padrão
long long obter_fitness_da_instancia(const char *caminho_arquivo);

void liberar_solucao(Solucao *sol);

#endif // PARTITION_H