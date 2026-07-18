#include <stdio.h>
#include <stdlib.h>
#include "pbm.h"
#include "chaincode.h"
#include "ctn.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.ctn> <imagem_saida.pbm>\n", argv[0]);
        return 1;
    }

    const char *caminho_entrada = argv[1];
    const char *caminho_saida = argv[2];

    ArquivoCtn *ctn = ler_ctn(caminho_entrada);
    if (!ctn) {
        fprintf(stderr, "Erro: nao foi possivel ler o arquivo '%s'.\n", caminho_entrada);
        return 1;
    }

    int *direcoes = converter_hex_para_codigo_cadeia(ctn->cadeia_hexadecimal, ctn->quantidade_direcoes);

    Imagem *imagem = reconstruir_imagem_a_partir_do_codigo(ctn->linhas, ctn->colunas,
                                                            ctn->linha_inicial, ctn->coluna_inicial,
                                                            direcoes, ctn->quantidade_direcoes);

    if (!escrever_pbm(caminho_saida, imagem)) {
        fprintf(stderr, "Erro: nao foi possivel gravar a imagem '%s'.\n", caminho_saida);
        free(direcoes);
        liberar_imagem(imagem);
        liberar_ctn(ctn);
        return 1;
    }

    printf("Decodificacao concluida.\n");
    printf("  Dimensoes (linhas colunas): %d %d\n", ctn->linhas, ctn->colunas);
    printf("  Ponto inicial: %d %d\n", ctn->linha_inicial, ctn->coluna_inicial);
    printf("  Numero de direcoes: %d\n", ctn->quantidade_direcoes);
    printf("  Imagem gerada: %s\n", caminho_saida);

    free(direcoes);
    liberar_imagem(imagem);
    liberar_ctn(ctn);
    return 0;
}
