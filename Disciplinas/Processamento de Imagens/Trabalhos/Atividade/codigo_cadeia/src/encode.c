#include <stdio.h>
#include <stdlib.h>
#include "pbm.h"
#include "chaincode.h"
#include "ctn.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <imagem_entrada.pbm> <arquivo_saida.ctn>\n", argv[0]);
        return 1;
    }

    const char *caminho_entrada = argv[1];
    const char *caminho_saida = argv[2];

    Imagem *imagem = ler_pbm(caminho_entrada);
    if (!imagem) {
        fprintf(stderr, "Erro: nao foi possivel ler a imagem PBM '%s'.\n", caminho_entrada);
        return 1;
    }

    CodigoCadeia *codigo = extrair_codigo_cadeia_da_imagem(imagem);

    char *cadeia_hex = converter_codigo_cadeia_para_hex(codigo->direcoes, codigo->quantidade_direcoes);

    if (!escrever_ctn(caminho_saida, imagem->linhas, imagem->colunas,
                       codigo->linha_inicial, codigo->coluna_inicial,
                       codigo->quantidade_direcoes, cadeia_hex)) {
        fprintf(stderr, "Erro: nao foi possivel gravar o arquivo '%s'.\n", caminho_saida);
        free(cadeia_hex);
        liberar_codigo_cadeia(codigo);
        liberar_imagem(imagem);
        return 1;
    }

    printf("Codificacao concluida.\n");
    printf("  Dimensoes (linhas colunas): %d %d\n", imagem->linhas, imagem->colunas);
    printf("  Ponto inicial: %d %d\n", codigo->linha_inicial, codigo->coluna_inicial);
    printf("  Numero de direcoes: %d\n", codigo->quantidade_direcoes);
    printf("  Cadeia hexadecimal: %s\n", cadeia_hex);
    printf("  Arquivo gerado: %s\n", caminho_saida);

    free(cadeia_hex);
    liberar_codigo_cadeia(codigo);
    liberar_imagem(imagem);
    return 0;
}
