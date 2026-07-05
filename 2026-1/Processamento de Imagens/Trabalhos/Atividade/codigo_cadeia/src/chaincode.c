#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "chaincode.h"

/*
 * Tabela de deslocamentos (linha, coluna):
 *
 *      3 2 1
 *      4 x 0
 *      5 6 7
 */
const Deslocamento TABELA_DESLOCAMENTOS[8] = {
    { 0,  1}, /* 0 Leste     */
    {-1,  1}, /* 1 Nordeste  */
    {-1,  0}, /* 2 Norte     */
    {-1, -1}, /* 3 Noroeste  */
    { 0, -1}, /* 4 Oeste     */
    { 1, -1}, /* 5 Sudoeste  */
    { 1,  0}, /* 6 Sul       */
    { 1,  1}, /* 7 Sudeste   */
};

/* Retorna o valor do pixel (0 ou 1). Fora dos limites da imagem = 0 (fundo). */
static int obter_pixel(const Imagem *imagem, int linha, int coluna) {
    if (linha < 0 || linha >= imagem->linhas || coluna < 0 || coluna >= imagem->colunas) {
        return 0;
    }
    return imagem->pixels[linha][coluna] ? 1 : 0;
}

void liberar_codigo_cadeia(CodigoCadeia *codigo) {
    if (!codigo) return;
    free(codigo->direcoes);
    free(codigo);
}

CodigoCadeia *extrair_codigo_cadeia_da_imagem(const Imagem *imagem) {
    CodigoCadeia *codigo = malloc(sizeof(CodigoCadeia));
    codigo->linha_inicial = 0;
    codigo->coluna_inicial = 0;
    codigo->quantidade_direcoes = 0;
    codigo->direcoes = NULL;

    /* Passo 1: localizar o ponto inicial (varredura esquerda->direita, cima->baixo) */
    int linha_inicial = -1, coluna_inicial = -1;
    for (int linha = 0; linha < imagem->linhas && linha_inicial < 0; linha++) {
        for (int coluna = 0; coluna < imagem->colunas; coluna++) {
            if (obter_pixel(imagem, linha, coluna)) {
                linha_inicial = linha;
                coluna_inicial = coluna;
                break;
            }
        }
    }

    if (linha_inicial < 0) {
        /* Nenhum pixel de contorno encontrado na imagem. */
        return codigo;
    }

    codigo->linha_inicial = linha_inicial;
    codigo->coluna_inicial = coluna_inicial;

    /* Caso especial: pixel isolado, sem nenhum vizinho de contorno.
     * A cadeia de direções fica vazia (tamanho 0). */
    int possui_vizinho = 0;
    for (int d = 0; d < 8; d++) {
        int linha_vizinha = linha_inicial + TABELA_DESLOCAMENTOS[d].delta_linha;
        int coluna_vizinha = coluna_inicial + TABELA_DESLOCAMENTOS[d].delta_coluna;
        if (obter_pixel(imagem, linha_vizinha, coluna_vizinha)) {
            possui_vizinho = 1;
            break;
        }
    }
    if (!possui_vizinho) {
        return codigo;
    }

    /* Passos 2 e 3: rastreamento do contorno por vizinhança de Moore. */
    int capacidade = 256;
    int *direcoes_encontradas = malloc(sizeof(int) * capacidade);
    int total_direcoes = 0;

    int linha_atual = linha_inicial, coluna_atual = coluna_inicial;
    int direcao_de_chegada = 0;

    long max_iteracoes = (long)imagem->linhas * (long)imagem->colunas * 8L + 8L;
    long iteracao = 0;

    while (iteracao++ < max_iteracoes) {
        int inicio_da_busca = (direcao_de_chegada + 5) % 8;
        int direcao_encontrada = -1;

        for (int k = 0; k < 8; k++) {
            int direcao_candidata = (inicio_da_busca + k) % 8;
            int linha_vizinha = linha_atual + TABELA_DESLOCAMENTOS[direcao_candidata].delta_linha;
            int coluna_vizinha = coluna_atual + TABELA_DESLOCAMENTOS[direcao_candidata].delta_coluna;
            if (obter_pixel(imagem, linha_vizinha, coluna_vizinha)) {
                direcao_encontrada = direcao_candidata;
                break;
            }
        }

        if (direcao_encontrada < 0) break; /* nenhum vizinho encontrado */

        if (total_direcoes >= capacidade) {
            capacidade *= 2;
            direcoes_encontradas = realloc(direcoes_encontradas, sizeof(int) * capacidade);
        }
        direcoes_encontradas[total_direcoes++] = direcao_encontrada;

        linha_atual += TABELA_DESLOCAMENTOS[direcao_encontrada].delta_linha;
        coluna_atual += TABELA_DESLOCAMENTOS[direcao_encontrada].delta_coluna;
        direcao_de_chegada = direcao_encontrada;

        if (linha_atual == linha_inicial && coluna_atual == coluna_inicial) {
            break; /* voltou ao ponto inicial: contorno fechado */
        }
    }

    codigo->quantidade_direcoes = total_direcoes;
    codigo->direcoes = direcoes_encontradas;
    return codigo;
}

Imagem *reconstruir_imagem_a_partir_do_codigo(int linhas, int colunas,
                                               int linha_inicial, int coluna_inicial,
                                               const int *direcoes,
                                               int quantidade_direcoes) {
    Imagem *imagem = criar_imagem(linhas, colunas);
    if (!imagem) return NULL;

    int linha_atual = linha_inicial, coluna_atual = coluna_inicial;
    if (linha_atual >= 0 && linha_atual < linhas && coluna_atual >= 0 && coluna_atual < colunas) {
        imagem->pixels[linha_atual][coluna_atual] = 1;
    }

    for (int i = 0; i < quantidade_direcoes; i++) {
        int direcao = direcoes[i];
        linha_atual += TABELA_DESLOCAMENTOS[direcao].delta_linha;
        coluna_atual += TABELA_DESLOCAMENTOS[direcao].delta_coluna;
        if (linha_atual >= 0 && linha_atual < linhas && coluna_atual >= 0 && coluna_atual < colunas) {
            imagem->pixels[linha_atual][coluna_atual] = 1;
        }
    }

    return imagem;
}

char *converter_codigo_cadeia_para_hex(const int *direcoes, int quantidade_direcoes) {
    /* Cada direção ocupa 3 bits. */
    int total_bits_uteis = quantidade_direcoes * 3;
    /* Arredonda para cima até o próximo múltiplo de 4 (tamanho de um dígito hex). */
    int quantidade_digitos_hex = (total_bits_uteis + 3) / 4;
    int total_bits = quantidade_digitos_hex * 4;

    /* Buffer de bits como caracteres '0'/'1' para facilitar o agrupamento
     * em blocos de 4 na hora de gerar cada dígito hexadecimal. */
    char *bits = malloc(total_bits + 1);
    memset(bits, '0', total_bits);
    bits[total_bits] = '\0';

    int posicao = 0;
    for (int i = 0; i < quantidade_direcoes; i++) {
        int direcao = direcoes[i];
        for (int bit = 2; bit >= 0; bit--) {
            bits[posicao++] = ((direcao >> bit) & 1) ? '1' : '0';
        }
    }
    
    /* Os bits restantes já estão em '0': são os bits de preenchimento (padding). */
    char *hexadecimal = malloc((size_t)quantidade_digitos_hex + 1);
    for (int i = 0; i < quantidade_digitos_hex; i++) {
        int valor_nibble = 0;
        for (int bit = 0; bit < 4; bit++) {
            valor_nibble = (valor_nibble << 1) | (bits[i * 4 + bit] - '0');
        }
        sprintf(&hexadecimal[i], "%X", valor_nibble);
    }
    hexadecimal[quantidade_digitos_hex] = '\0';

    free(bits);
    return hexadecimal;
}

/* Converte um caractere hexadecimal ('0'-'9', 'A'-'F' ou 'a'-'f') no seu valor numérico (0-15). */
static int valor_do_digito_hex(char caractere) {
    caractere = (char)toupper((unsigned char)caractere);
    if (caractere >= '0' && caractere <= '9') return caractere - '0';
    if (caractere >= 'A' && caractere <= 'F') return caractere - 'A' + 10;
    return 0;
}

int *converter_hex_para_codigo_cadeia(const char *hexadecimal, int quantidade_direcoes) {
    int quantidade_digitos = (int)strlen(hexadecimal);
    int total_bits = quantidade_digitos * 4;

    /* Expande cada dígito hexadecimal para 4 bits (caracteres '0'/'1'). */
    char *bits = malloc((size_t)total_bits + 1);
    for (int i = 0; i < quantidade_digitos; i++) {
        int valor = valor_do_digito_hex(hexadecimal[i]);
        for (int bit = 0; bit < 4; bit++) {
            bits[i * 4 + bit] = ((valor >> (3 - bit)) & 1) ? '1' : '0';
        }
    }
    bits[total_bits] = '\0';

    /* Agrupa os bits em blocos de 3 para recuperar cada direção,
     * descartando os bits de preenchimento no final, se sobrarem. */
    int *direcoes = malloc(sizeof(int) * (quantidade_direcoes > 0 ? quantidade_direcoes : 1));
    for (int i = 0; i < quantidade_direcoes; i++) {
        int valor = 0;
        for (int bit = 0; bit < 3; bit++) {
            int indice = i * 3 + bit;
            int valor_bit = (indice < total_bits) ? (bits[indice] - '0') : 0;
            valor = (valor << 1) | valor_bit;
        }
        direcoes[i] = valor;
    }

    free(bits);
    return direcoes;
}
