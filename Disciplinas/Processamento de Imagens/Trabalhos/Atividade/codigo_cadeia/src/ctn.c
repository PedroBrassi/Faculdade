#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctn.h"

int escrever_ctn(const char *nome_arquivo, int linhas, int colunas,
                  int linha_inicial, int coluna_inicial,
                  int quantidade_direcoes, const char *cadeia_hexadecimal) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) return 0;

    fprintf(arquivo, "%d %d\n", linhas, colunas);
    fprintf(arquivo, "%d %d\n", linha_inicial, coluna_inicial);
    fprintf(arquivo, "%d\n", quantidade_direcoes);
    fprintf(arquivo, "%s\n", (quantidade_direcoes > 0) ? cadeia_hexadecimal : "");

    fclose(arquivo);
    return 1;
}

ArquivoCtn *ler_ctn(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) return NULL;

    ArquivoCtn *ctn = malloc(sizeof(ArquivoCtn));
    ctn->cadeia_hexadecimal = NULL;

    if (fscanf(arquivo, "%d %d", &ctn->linhas, &ctn->colunas) != 2) {
        fclose(arquivo); free(ctn); return NULL;
    }
    if (fscanf(arquivo, "%d %d", &ctn->linha_inicial, &ctn->coluna_inicial) != 2) {
        fclose(arquivo); free(ctn); return NULL;
    }
    if (fscanf(arquivo, "%d", &ctn->quantidade_direcoes) != 1) {
        fclose(arquivo); free(ctn); return NULL;
    }

    char buffer_hex[8192] = {0};
    if (ctn->quantidade_direcoes > 0) {
        if (fscanf(arquivo, "%8191s", buffer_hex) != 1) {
            fclose(arquivo); free(ctn); return NULL;
        }
    }
    ctn->cadeia_hexadecimal = strdup(buffer_hex);

    fclose(arquivo);
    return ctn;
}

void liberar_ctn(ArquivoCtn *ctn) {
    if (!ctn) return;
    free(ctn->cadeia_hexadecimal);
    free(ctn);
}
