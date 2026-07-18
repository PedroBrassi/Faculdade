#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "pbm.h"

Imagem *criar_imagem(int linhas, int colunas) {
    Imagem *imagem = malloc(sizeof(Imagem));
    if (!imagem) return NULL;

    imagem->linhas = linhas;
    imagem->colunas = colunas;
    imagem->pixels = malloc(sizeof(unsigned char *) * linhas);
    if (!imagem->pixels) { free(imagem); return NULL; }

    for (int linha = 0; linha < linhas; linha++) {
        imagem->pixels[linha] = calloc(colunas, sizeof(unsigned char));
    }
    return imagem;
}

void liberar_imagem(Imagem *imagem) {
    if (!imagem) return;
    if (imagem->pixels) {
        for (int linha = 0; linha < imagem->linhas; linha++) {
            free(imagem->pixels[linha]);
        }
        free(imagem->pixels);
    }
    free(imagem);
}

/* Pula espaços em branco e comentários (linhas iniciadas por '#'). */
static void pular_espacos_e_comentarios(FILE *arquivo) {
    int caractere;
    while ((caractere = fgetc(arquivo)) != EOF) {
        if (isspace(caractere)) {
            continue;
        } else if (caractere == '#') {
            while ((caractere = fgetc(arquivo)) != EOF && caractere != '\n')
                ;
        } else {
            ungetc(caractere, arquivo);
            break;
        }
    }
}

/* Lê um inteiro do arquivo, pulando espaços/comentários antes dele. */
static int ler_token_inteiro(FILE *arquivo, int *saida) {
    pular_espacos_e_comentarios(arquivo);
    return fscanf(arquivo, "%d", saida) == 1;
}

Imagem *ler_pbm(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) return NULL;

    char assinatura[3] = {0};
    pular_espacos_e_comentarios(arquivo);
    if (fscanf(arquivo, "%2s", assinatura) != 1) { fclose(arquivo); return NULL; }

    if (assinatura[0] != 'P' || (assinatura[1] != '1' && assinatura[1] != '4')) {
        fclose(arquivo);
        return NULL;
    }

    int colunas, linhas;
    if (!ler_token_inteiro(arquivo, &colunas) || !ler_token_inteiro(arquivo, &linhas)) {
        fclose(arquivo);
        return NULL;
    }

    Imagem *imagem = criar_imagem(linhas, colunas);
    if (!imagem) { fclose(arquivo); return NULL; }

    if (assinatura[1] == '1') {
        /* Formato ASCII: cada pixel é um caractere '0' ou '1'. */
        for (int linha = 0; linha < linhas; linha++) {
            for (int coluna = 0; coluna < colunas; coluna++) {
                int valor;
                if (!ler_token_inteiro(arquivo, &valor)) {
                    liberar_imagem(imagem);
                    fclose(arquivo);
                    return NULL;
                }
                imagem->pixels[linha][coluna] = (valor != 0) ? 1 : 0;
            }
        }
    } else {
        /* Formato binário P4: um único espaço/whitespace após o header,
         * depois os pixels empacotados em bits, MSB primeiro, cada
         * linha alinhada em byte. */
        fgetc(arquivo); /* consome o whitespace obrigatório após 'linhas' */

        int bytes_por_linha = (colunas + 7) / 8;
        unsigned char *buffer_linha = malloc(bytes_por_linha);

        for (int linha = 0; linha < linhas; linha++) {
            if (fread(buffer_linha, 1, bytes_por_linha, arquivo) != (size_t)bytes_por_linha) {
                free(buffer_linha);
                liberar_imagem(imagem);
                fclose(arquivo);
                return NULL;
            }
            for (int coluna = 0; coluna < colunas; coluna++) {
                int indice_byte = coluna / 8;
                int indice_bit = 7 - (coluna % 8);
                int bit = (buffer_linha[indice_byte] >> indice_bit) & 1;
                imagem->pixels[linha][coluna] = (unsigned char)bit;
            }
        }
        free(buffer_linha);
    }

    fclose(arquivo);
    return imagem;
}

int escrever_pbm(const char *nome_arquivo, const Imagem *imagem) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) return 0;

    fprintf(arquivo, "P1\n");
    fprintf(arquivo, "# Gerado pelo decodificador de codigo de cadeia\n");
    fprintf(arquivo, "%d %d\n", imagem->colunas, imagem->linhas);

    for (int linha = 0; linha < imagem->linhas; linha++) {
        for (int coluna = 0; coluna < imagem->colunas; coluna++) {
            fprintf(arquivo, "%d", imagem->pixels[linha][coluna] ? 1 : 0);
            if (coluna != imagem->colunas - 1) fputc(' ', arquivo);
        }
        fputc('\n', arquivo);
    }

    fclose(arquivo);
    return 1;
}
