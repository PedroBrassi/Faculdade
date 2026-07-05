#ifndef CTN_H
#define CTN_H

/*
 * Representa o conteúdo de um arquivo .ctn:
 *
 *   <linhas> <colunas>
 *   <linha_inicial> <coluna_inicial>
 *   <quantidade_direcoes>
 *   <cadeia_hexadecimal>
 */
typedef struct {
    int linhas, colunas;
    int linha_inicial, coluna_inicial;
    int quantidade_direcoes;
    char *cadeia_hexadecimal;
} ArquivoCtn;

/* Grava um arquivo .ctn. Retorna 1 em sucesso, 0 em erro. */
int escrever_ctn(const char *nome_arquivo, int linhas, int colunas,
                  int linha_inicial, int coluna_inicial,
                  int quantidade_direcoes, const char *cadeia_hexadecimal);

/* Lê um arquivo .ctn. Retorna NULL em caso de erro. */
ArquivoCtn *ler_ctn(const char *nome_arquivo);

void liberar_ctn(ArquivoCtn *ctn);

#endif /* CTN_H */
