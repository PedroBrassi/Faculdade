#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/genetico.h"
#include "../include/partition.h"

/**
 * Funcao auxiliar estatica para copiar o conteudo de uma struct Solucao para outra.
 * Copia o valor do fitness e clona o vetor de configuracao (subconjuntos).
 */
static void copiar_solucao_ag(Solucao *destino, const Solucao *origem, int n) {
    destino->fitness = origem->fitness; // Copia a qualidade obtida
    for (int i = 0; i < n; i++) {
        destino->config[i] = origem->config[i]; // Copia a alocacao elemento a elemento
    }
}

/**
 * Selecao por Torneio
 * Sorteia 'k' (3) individuos aleatorios da populacao e retorna o indice daquele
 * que possuir o MENOR fitness (como e um problema de minimizacao, menor e melhor).
 */
static int selecao_torneio(Solucao *populacao, int tam_pop) {
    int k = 3; // Tamanho do torneio (quantidade de competidores)
    int melhor_idx = rand() % tam_pop; // Seleciona o primeiro competidor aleatoriamente
    
    for (int i = 1; i < k; i++) {
        int candidato_idx = rand() % tam_pop; // Sorteia outro competidor
        // Se o candidato atual for melhor (menor fitness) que o melhor guardado até agora
        if (populacao[candidato_idx].fitness < populacao[melhor_idx].fitness) {
            melhor_idx = candidato_idx; // Atualiza o indice do vencedor
        }
    }
    return melhor_idx; // Retorna o vencedor do torneio para ser pai/mae
}

/**
 * Execucao principal da Meta-heuristica do Algoritmo Genetico (AG)
 */
Solucao executar_algoritmo_genetico(const Instancia *inst) {
    
    // --- 1. DEFINICAO DOS PARAMETROS DO ALGORITMO ---
    // Define o tamanho da populacao de acordo com a complexidade do problema
    int TAM_POP  = (inst->n < 500) ? 50 : 100;
    
    // [MELHORIA] Escalabilidade: Gerações ajustadas dinamicamente para N grande.
    // Evita gastar tempo excessivo em N pequeno e dá fôlego de convergência para N grande.
    int GERACOES;
    if (inst->n <= 500)
        GERACOES = 100;
    else if (inst->n <= 5000)
        GERACOES = 300;
    else
        GERACOES = 600; // Para N > 5000, mais gerações permitem explorar melhor o espaco de busca
    
    double TX_CROSSOVER = 0.85; // 85% de chance de ocorrer cruzamento entre os pais
    
    // Taxa de mutacao base calculada de forma ponderada (em media, altera 2 bits por filho gerado)
    double TX_MUTACAO_BASE = 2.0 / inst->n; 

    // --- 2. ALOCACAO E INICIALIZACAO DA POPULACAO ---
    // Aloca os arrays que representam a populacao atual e o buffer da proxima geracao
    Solucao *populacao = (Solucao *)malloc(TAM_POP * sizeof(Solucao));
    Solucao *nova_populacao = (Solucao *)malloc(TAM_POP * sizeof(Solucao));
    
    // Alocacao da estrutura que guardara a melhor solucao encontrada na história do algoritmo
    Solucao melhor_global;
    melhor_global.config = (int *)malloc(inst->n * sizeof(int));
    melhor_global.fitness = -1; // -1 indica que ainda nao foi inicializada

    // Cria os individuos iniciais (Geracao 0) com configuracoes 100% aleatorias
    for (int i = 0; i < TAM_POP; i++) {
        populacao[i].config = (int *)malloc(inst->n * sizeof(int));
        nova_populacao[i].config = (int *)malloc(inst->n * sizeof(int));
        
        for (int j = 0; j < inst->n; j++) {
            populacao[i].config[j] = rand() % 2; // Distribui aleatoriamente no Grupo 0 ou Grupo 1
        }
        
        avaliar_solucao(&populacao[i], inst); // Calcula o fitness do individuo gerado
        
        // Mantem o registro atualizado do melhor individuo global inicial
        if (melhor_global.fitness == -1 || populacao[i].fitness < melhor_global.fitness) {
            copiar_solucao_ag(&melhor_global, &populacao[i], inst->n);
        }
    }

    // --- 3. LOOP EVOLUTIVO (CICLO DE GERACOES) ---
    for (int g = 0; g < GERACOES; g++) {
        
        // Criterio de parada prematura: se alcançou a solucao otima perfeita (diferenca zero), interrompe
        if (melhor_global.fitness == 0) break;

        // [MELHORIA] Mutação Adaptativa (Mecanismo anti-convergência prematura)
        // Se o algoritmo entrar nas últimas 20% das geracoes, triplica a taxa de mutacao
        // para sacudir a populacao e tentar escapar de eventuais minimos locais.
        double mut_factor = 1.0;
        if (g > 0.8 * GERACOES) {
            mut_factor = 3.0; // Torna o algoritmo mais agressivo na busca fina/final
        }
        double TX_MUTACAO_ATUAL = TX_MUTACAO_BASE * mut_factor;

        // --- ELITISMO ---
        // Identifica quem e o melhor individuo da populacao atual
        int melhor_geracao_idx = 0;
        for (int i = 1; i < TAM_POP; i++) {
            if (populacao[i].fitness < populacao[melhor_geracao_idx].fitness) {
                melhor_geracao_idx = i;
            }
        }
        
        // Se o lider desta geracao for melhor que o lider de toda a historia, atualiza o Historico Global
        if (populacao[melhor_geracao_idx].fitness < melhor_global.fitness) {
            copiar_solucao_ag(&melhor_global, &populacao[melhor_geracao_idx], inst->n);
        }
        
        // Garante o Elitismo Puro: Copia obrigatoriamente o melhor da atual para a posicao 0 da nova populacao
        copiar_solucao_ag(&nova_populacao[0], &populacao[melhor_geracao_idx], inst->n);

        // --- REPRODUCAO (CRUZAMENTO E MUTACAO) ---
        // Preenche o restante da nova populacao de 2 em 2 filhos
        for (int i = 1; i < TAM_POP; i += 2) {
            
            // Seleciona os dois pais utilizando o torneio de 3 competidores
            int pai1_idx = selecao_torneio(populacao, TAM_POP);
            int pai2_idx = selecao_torneio(populacao, TAM_POP);

            // Tratamento de salvaguarda: Se a populacao for impar e restou apenas 1 vaga na nova geracao, 
            // apenas clona o pai selecionado e encerra o loop de reproducao.
            if (i == TAM_POP - 1) {
                copiar_solucao_ag(&nova_populacao[i], &populacao[pai1_idx], inst->n);
                break;
            }

            // Testa se o Crossover vai acontecer com base na taxa estipulada
            double r_cross = (double)rand() / RAND_MAX;
            if (r_cross < TX_CROSSOVER) {
                // Executa Crossover de Um Ponto (Single-Point Crossover)
                int ponto_corte = rand() % inst->n; // Sorteia a posicao de corte no cromossomo
                
                for (int j = 0; j < inst->n; j++) {
                    if (j < ponto_corte) {
                        // Antes do corte: Filho 1 herda do Pai 1, Filho 2 herda do Pai 2
                        nova_populacao[i].config[j] = populacao[pai1_idx].config[j];
                        nova_populacao[i+1].config[j] = populacao[pai2_idx].config[j];
                    } else {
                        // Apos o corte: Inverte a heranca (Filho 1 herda do Pai 2, Filho 2 herda do Pai 1)
                        nova_populacao[i].config[j] = populacao[pai2_idx].config[j];
                        nova_populacao[i+1].config[j] = populacao[pai1_idx].config[j];
                    }
                }
            } else {
                // Se nao houver crossover, os filhos sao copias exatas e integras dos pais
                copiar_solucao_ag(&nova_populacao[i], &populacao[pai1_idx], inst->n);
                copiar_solucao_ag(&nova_populacao[i+1], &populacao[pai2_idx], inst->n);
            }

            // --- MUTACAO POR FLIP BIT ---
            // Percorre cada gene (posicao do vetor) dos dois filhos gerados
            for (int j = 0; j < inst->n; j++) {
                // Se o sorteio for menor que a taxa adaptativa, inverte o grupo do elemento (0 vira 1, 1 vira 0)
                if (((double)rand() / RAND_MAX) < TX_MUTACAO_ATUAL) {
                    nova_populacao[i].config[j] = !nova_populacao[i].config[j];
                }
                if (((double)rand() / RAND_MAX) < TX_MUTACAO_ATUAL) {
                    nova_populacao[i+1].config[j] = !nova_populacao[i+1].config[j];
                }
            }
            
            // Recalcula o fitness dos novos filhos gerados e mutados
            avaliar_solucao(&nova_populacao[i], inst);
            avaliar_solucao(&nova_populacao[i+1], inst);
        }

        // --- SUBSTITUICAO GENERACIONAL ---
        // Copia em bloco toda a nova populacao gerada de volta para o vetor principal
        for (int i = 0; i < TAM_POP; i++) {
            copiar_solucao_ag(&populacao[i], &nova_populacao[i], inst->n);
        }
    }

    // --- 4. LIBERACAO DE MEMORIA (ANTI-MEMORY LEAK) ---
    // Desaloca as configuracoes internas de cada individuo das populacoes de trabalho
    for (int i = 0; i < TAM_POP; i++) {
        free(populacao[i].config);
        free(nova_populacao[i].config);
    }
    // Libera os ponteiros das estruturas de vetores
    free(populacao);
    free(nova_populacao);

    // Retorna a melhor solucao capturada ao longo de todo o processo evolutivo
    return melhor_global;
}