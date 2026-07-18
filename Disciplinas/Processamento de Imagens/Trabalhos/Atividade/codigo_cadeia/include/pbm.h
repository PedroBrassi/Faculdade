#ifndef PBM_H
#define PBM_H

/*
 * Representa uma imagem binária (valores 0 ou 1) em memória.
 * pixels[linha][coluna], com 'linhas' linhas e 'colunas' colunas.
 */
typedef struct {
    int linhas;
    int colunas;
    unsigned char **pixels;
} Imagem;

/* Cria uma imagem linhas x colunas com todos os pixels em 0. */
Imagem *criar_imagem(int linhas, int colunas);

/* Libera toda a memória associada a uma imagem. */
void liberar_imagem(Imagem *imagem);

/*
 * Lê uma imagem no formato PBM (suporta P1 - ASCII e P4 - binário).
 * Retorna NULL em caso de erro (arquivo inexistente, formato inválido, etc).
 */
Imagem *ler_pbm(const char *nome_arquivo);

/*
 * Grava uma imagem no formato PBM ASCII (P1).
 * Retorna 1 em caso de sucesso e 0 em caso de erro.
 */
int escrever_pbm(const char *nome_arquivo, const Imagem *imagem);

#endif /* PBM_H */
