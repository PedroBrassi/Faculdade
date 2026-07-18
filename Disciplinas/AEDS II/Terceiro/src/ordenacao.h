#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <string.h>
#include <stdlib.h>
#include "leitura.h"

// 1 e 2
void selectionSort(Jogador *vet, int n, long long *comp, long long *troca, long long *mem);
void mergeSort(Jogador *vet, int ini, int fim, long long *comp, long long *troca, long long *mem);

// 3 e 3.1
void radixSortNomes(Jogador *vet, int n, long long *comp, long long *troca, long long *mem);
void countingSortRadix(Jogador *vet, int n, int pos, long long *comp, long long *troca, long long *mem);

// 1) SELECTION SORT - Simples
void selectionSort(Jogador *vet, int n, long long *comp, long long *troca, long long *mem) {

    for (int i = 0; i < n - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < n; j++) {
            (*comp)++;
            if (strcmp(vet[j].nome, vet[menor].nome) < 0)
                menor = j;
        }

        if (menor != i) {
            Jogador temp = vet[i];
            vet[i] = vet[menor];
            vet[menor] = temp;
            (*troca)++;
        }
    }
}

// 2) MERGE SORT - Ótimo
static void mergeInter(Jogador *vet, int ini, int meio, int fim,
                       long long *comp, long long *troca, long long *mem) {

    int n1 = meio - ini + 1;
    int n2 = fim - meio;

    Jogador *L = malloc(n1 * sizeof(Jogador));
    Jogador *R = malloc(n2 * sizeof(Jogador));

    *mem += (n1 + n2) * sizeof(Jogador);

    for (int i = 0; i < n1; i++) L[i] = vet[ini + i];
    for (int j = 0; j < n2; j++) R[j] = vet[meio + 1 + j];

    int i = 0, j = 0, k = ini;

    while (i < n1 && j < n2) {

        (*comp)++;
        if (strcmp(L[i].nome, R[j].nome) <= 0)
            vet[k++] = L[i++];
        else
            vet[k++] = R[j++];

        (*troca)++;
    }

    while (i < n1) vet[k++] = L[i++], (*troca)++;
    while (j < n2) vet[k++] = R[j++], (*troca)++;

    free(L);
    free(R);
}

void mergeSort(Jogador *vet, int ini, int fim, long long *comp, long long *troca, long long *mem) {

    if (ini >= fim) return;

    int meio = (ini + fim) / 2;

    mergeSort(vet, ini, meio, comp, troca, mem);
    mergeSort(vet, meio + 1, fim, comp, troca, mem);

    mergeInter(vet, ini, meio, fim, comp, troca, mem);
}

// 3) COUNTING SORT PARA RADIX - Linear
void countingSortRadix(Jogador *vet, int n, int pos,
                       long long *comp, long long *troca, long long *mem) {

    int K = 257; // 0 (vazio) + 256 chars possíveis

    int *count = calloc(K, sizeof(int));
    if (!count) {
        fprintf(stderr, "Erro no calloc do countingSortRadix\n");
        exit(1);
    }
    *mem += K * sizeof(int);

    Jogador *saida = malloc(n * sizeof(Jogador));
    if (!saida) {
        fprintf(stderr, "Erro no malloc do countingSortRadix\n");
        free(count);
        exit(1);
    }
    *mem += n * sizeof(Jogador);

    // 1) Contagem
    for (int i = 0; i < n; i++) {
        int tam = strlen(vet[i].nome);
        int idx = tam - 1 - pos;

        unsigned char chave = 0;
        if (idx >= 0) chave = (unsigned char)vet[i].nome[idx];

        count[chave]++;
    }

    // 2) Prefixos
    for (int i = 1; i < K; i++)
        count[i] += count[i - 1];

    // 3) Construção estável
    for (int i = n - 1; i >= 0; i--) {
        int tam = strlen(vet[i].nome);
        int idx = tam - 1 - pos;

        unsigned char chave = 0;
        if (idx >= 0) chave = (unsigned char)vet[i].nome[idx];

        saida[--count[chave]] = vet[i];
        (*troca)++;
    }

    // 4) Copiar de volta
    for (int i = 0; i < n; i++)
        vet[i] = saida[i];

    free(count);
    free(saida);
}


// 3.1) RADIX SORT COMPLETO POR NOME
void radixSortNomes(Jogador *vet, int n,
                    long long *comp, long long *troca, long long *mem) {

    int maxTam = 0;

    for (int i = 0; i < n; i++) {
        int t = strlen(vet[i].nome);
        if (t > maxTam)
            maxTam = t;
    }

    for (int pos = 0; pos < maxTam; pos++)
        countingSortRadix(vet, n, pos, comp, troca, mem);
}

#endif
