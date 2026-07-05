#ifndef CODIGO_CADEIA_H
#define CODIGO_CADEIA_H

#include "pbm.h"

/* Deslocamento (linha, coluna) associado a cada direção 0..7. */
typedef struct {
    int delta_linha;
    int delta_coluna;
} Deslocamento;

/*
 * Tabela de deslocamentos na atividade:
 *
 *      3 2 1
 *      4 x 0
 *      5 6 7
 *
 * 0 Leste, 1 Nordeste, 2 Norte, 3 Noroeste,
 * 4 Oeste, 5 Sudoeste, 6 Sul, 7 Sudeste.
 */
extern const Deslocamento TABELA_DESLOCAMENTOS[8];

/* Resultado do rastreamento de contorno de uma imagem. */
typedef struct {
    int linha_inicial;
    int coluna_inicial;
    int quantidade_direcoes;
    int *direcoes; /* vetor com códigos de direção, valores de 0 a 7 */
} CodigoCadeia;

/*
 * Extrai o código de cadeia do primeiro contorno encontrado na imagem
 * (busca por varredura, de cima para baixo, esquerda para direita).
 * Usa rastreamento de contorno por vizinhança (8-conectividade).
 */
CodigoCadeia *extrair_codigo_cadeia_da_imagem(const Imagem *imagem);

/*
 * Reconstrói uma imagem linhas x colunas a partir do ponto inicial e
 * da sequência de direções fornecida.
 */
Imagem *reconstruir_imagem_a_partir_do_codigo(int linhas, int colunas,
                                               int linha_inicial, int coluna_inicial,
                                               const int *direcoes,
                                               int quantidade_direcoes);

/*
 * Converte a sequência de direções (cada uma ocupando 3 bits) para uma
 * string hexadecimal (maiúscula), preenchendo com zeros à direita até
 * completar um múltiplo de 4 bits.
 * O chamador é responsável por liberar a string retornada.
 */
char *converter_codigo_cadeia_para_hex(const int *direcoes, int quantidade_direcoes);

/*
 * Converte uma string hexadecimal de volta para os primeiros
 * "quantidade_direcoes" códigos de direção (descarta os bits de
 * preenchimento finais).
 * O chamador é responsável por liberar o vetor retornado.
 */
int *converter_hex_para_codigo_cadeia(const char *hexadecimal, int quantidade_direcoes);

void liberar_codigo_cadeia(CodigoCadeia *codigo);

#endif /* CODIGO_CADEIA_H */
