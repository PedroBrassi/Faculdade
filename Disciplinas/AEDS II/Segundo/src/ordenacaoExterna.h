#ifndef ORDENACAO_EXTERNA_H
#define ORDENACAO_EXTERNA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// ===========================
// Definições de constantes
// ===========================
#define BYTES_POR_GB 1073741824  // 1 GB em bytes
#define MAX_BLOCOS 100           // Número máximo de blocos que podem ser fundidos

// =========================================================
// Função auxiliar do Merge Sort
// Combina duas metades ordenadas em um único vetor ordenado
// =========================================================
void merge(int *arr, int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    int *esq = (int *) malloc(n1 * sizeof(int));
    int *dir = (int *) malloc(n2 * sizeof(int));

    if (!esq || !dir) {
        printf("Erro de alocação na função merge!\n");
        exit(1);
    }

    for (int i = 0; i < n1; i++) esq[i] = arr[inicio + i];
    for (int j = 0; j < n2; j++) dir[j] = arr[meio + 1 + j];

    int i = 0, j = 0, k = inicio;

    // Combina as duas metades de forma ordenada
    while (i < n1 && j < n2) {
        if (esq[i] <= dir[j])
            arr[k++] = esq[i++];
        else
            arr[k++] = dir[j++];
    }

    // Copia o restante dos elementos
    while (i < n1) arr[k++] = esq[i++];
    while (j < n2) arr[k++] = dir[j++];

    free(esq);
    free(dir);
}

// =====================================
// Implementação recursiva do Merge Sort
// =====================================
void mergeSort(int *arr, int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        mergeSort(arr, inicio, meio);
        mergeSort(arr, meio + 1, fim);
        merge(arr, inicio, meio, fim);
    }
}

// ==============================================================
// Fusão externa dos blocos ordenados
// Combina os arquivos binários resultantes da etapa de ordenação
// Cria também uma saída em formato .txt
// ==============================================================
void fusaoExterna(int numBlocos) {
    FILE *blocos[MAX_BLOCOS];
    int buffer[MAX_BLOCOS];
    int ativo[MAX_BLOCOS];
    char nomeBloco[50];

    // Abre os arquivos de blocos binários
    for (int i = 0; i < numBlocos; i++) {
        sprintf(nomeBloco, "bloco_ordenado_%d.bin", i + 1);
        blocos[i] = fopen(nomeBloco, "rb");
        if (!blocos[i]) {
            printf("Erro ao abrir %s!\n", nomeBloco);
            exit(1);
        }
        // Lê o primeiro valor de cada bloco
        if (fread(&buffer[i], sizeof(int), 1, blocos[i]) == 1)
            ativo[i] = 1;
        else
            ativo[i] = 0;
    }

    FILE *saidaBin = fopen("resultado_final.bin", "wb");
    FILE *saidaTxt = fopen("resultado_final.txt", "w");

    if (!saidaBin || !saidaTxt) {
        printf("Erro ao criar arquivos de saída!\n");
        exit(1);
    }

    printf("\nIniciando fusão externa...\n");

    while (1) {
        int menor = INT_MAX;
        int indiceMenor = -1;

        // Encontra o menor valor entre os blocos ainda ativos
        for (int i = 0; i < numBlocos; i++) {
            if (ativo[i] && buffer[i] < menor) {
                menor = buffer[i];
                indiceMenor = i;
            }
        }

        // Se não há mais blocos ativos, encerra
        if (indiceMenor == -1) break;

        fwrite(&menor, sizeof(int), 1, saidaBin);
        fprintf(saidaTxt, "%d\n", menor);

        // Lê o próximo valor do bloco que forneceu o menor
        if (fread(&buffer[indiceMenor], sizeof(int), 1, blocos[indiceMenor]) != 1)
            ativo[indiceMenor] = 0;
    }

    // Fecha todos os arquivos
    for (int i = 0; i < numBlocos; i++)
        fclose(blocos[i]);
    fclose(saidaBin);
    fclose(saidaTxt);

    printf("Fusão concluída!\n");
    printf("Arquivos finais gerados:\n");
    printf(" - resultado_final.bin\n");
    printf(" - resultado_final.txt\n");
}


#endif
