#include <stdio.h>
#include <stdlib.h>
#include "../include/leitor.h"

/**
 * Funcao responsavel por abrir o arquivo de entrada, ler os dados da instancia,
 * alocar dinamicamente a estrutura e calcular dados agregados (soma_total).
 * Retorna um ponteiro para a Instancia alocada ou NULL em caso de falha.
 */
Instancia* ler_instancia(const char *caminho_arquivo) {
    // --- 1. ABERTURA DO ARQUIVO ---
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo: %s\n", caminho_arquivo);
        return NULL; // Falha inicial: nenhum recurso foi alocado ainda
    }

    // --- 2. ALOCACAO DA ESTRUTURA PRINCIPAL ---
    Instancia *inst = (Instancia *)malloc(sizeof(Instancia));
    if (inst == NULL) {
        printf("Erro de memoria ao alocar Instancia.\n");
        fclose(arquivo); // Fecha o arquivo aberto para evitar vazamento de descritor (resource leak)
        return NULL;
    }

    // --- 3. LEITURA DO TAMANHO DA INSTANCIA (N) ---
    // O arquivo deve comecar com um inteiro que define a quantidade de elementos
    if (fscanf(arquivo, "%d", &inst->n) != 1) {
        printf("Erro ao ler o tamanho 'n'.\n");
        free(inst);      // Desaloca a estrutura inst criada no passo anterior
        fclose(arquivo); // Fecha o arquivo
        return NULL;
    }

    // --- 4. ALOCACAO DINAMICA DO VETOR DE VALORES ---
    // Aloca exatamente o espaco necessario para armazenar os 'n' inteiros da instancia
    inst->valores = (int *)malloc(inst->n * sizeof(int));
    if (inst->valores == NULL) {
        printf("Erro de memoria ao alocar valores.\n");
        free(inst);      // Desaloca a estrutura principal
        fclose(arquivo); // Fecha o arquivo
        return NULL;
    }

    // --- 5. LEITURA DOS ELEMENTOS E CALCULO DA SOMA TOTAL ---
    inst->soma_total = 0; // Inicializa o acumulador com zero
    for (int i = 0; i < inst->n; i++) {
        // Le o i-esimo inteiro do arquivo e armazena na posicao correta do vetor
        if (fscanf(arquivo, "%d", &inst->valores[i]) != 1) {
            printf("Erro ao ler o elemento %d.\n", i);
            // Cascata de desalocacao segura: libera o que ja foi alocado de dentro para fora
            free(inst->valores); 
            free(inst);          
            fclose(arquivo);     
            return NULL;
        }
        // [OTIMIZACAO] Acumula o valor lido na soma total da instancia.
        // Fazer isso no leitor economiza loops adicionais no restante do programa.
        inst->soma_total += inst->valores[i];
    }

    // --- 6. FINALIZACAO ---
    fclose(arquivo); // Fecha o arquivo de forma limpa, pois a leitura foi bem-sucedida
    return inst;     // Retorna a estrutura totalmente preenchida pronta para uso pelos algoritmos
}

/**
 * Funcao de desalocacao obrigatoria (Destrutor).
 * Libera de forma segura toda a memoria alocada para a instancia,
 * evitando problemas de vazamento de memoria (Memory Leaks).
 */
void liberar_instancia(Instancia *inst) {
    if (inst != NULL) {
        // Verifica se o vetor interno de valores foi alocado antes de libera-lo
        if (inst->valores != NULL) {
            free(inst->valores); // Desaloca primeiro o vetor interno
        }
        free(inst); // Por fim, desaloca a estrutura que o continha
    }
}