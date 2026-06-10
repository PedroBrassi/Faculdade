/*================================================
        UNIFAL - Universidade Federal de Alfenas.
        BACHARELADO EM CIENCIA DA COMPUTACAO.
Trabalho..: Decodificao e codificacao de Código de Cadeia
Professor.: Luiz Eduardo da Silva
Aluno.....: Pedro Brassi Luccas
Data......: 10/06/2026
================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG(x)

typedef int *image;

typedef struct
{
    int i, j;
} point;

image img_alloc(int nl, int nc)
{
    return (image)malloc(nl * nc * sizeof(int));
}

void write_pbm(image img, char *nome, int nl, int nc)
{
    int i, conta;
    FILE *arq;
    if (!img)
    {
        printf("Erro: imagem vazia\n\n");
        exit(100);
    }
    if ((arq = fopen(nome, "wt")) == NULL)
    {
        printf("Erro: CRIACAO do arquivo <%s>\n\n", nome);
        exit(200);
    }
    fputs("P1\n", arq);
    fputs("#contour decoding image\n", arq);
    fprintf(arq, "%d  %d\n", nc, nl);

    conta = 0;
    for (i = 0; i < nl * nc; i++)
    {
        fprintf(arq, "%d ", img[i]);
        conta++;
        if (conta > 100)
        {
            fprintf(arq, "\n");
            conta = 0;
        }
    }
    fclose(arq);
}

image read_contour(char *nome, int *nl, int *nc)
{
    image img = NULL;
    FILE *f;
    f = fopen(nome, "rt");
    if (f == NULL)
    {
        printf("Erro: LEITURA do arquivo <%s>\n\n", nome);
        exit(100);
    }
    // Codigo 8-direcional
    // 3 2 1
    // 4 x 0
    // 5 6 7
    point N[8] = 
    {{0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, 
     {0, -1}, {1, -1}, {1, 0}, {1, 1}};

    // ----------
    // TO DO
    // ----------

    fclose(f);
    return img;
}

int main(int argc, char *argv[])
{
    int nl, nc;
    image in;
    char nome[20];
    char nomeimg[100];
    char nomectn[100];
    if (argc < 2)
    {
        printf("USO: %s <nomearquivo> (sem a extensao)\n\n", argv[0]);
        strcpy(nome, "teste");
        //return 1;
    }
    else
    {
        strcpy(nome, argv[1]);
    }
    sprintf(nomeimg, "%s-new.pbm", nome);
    sprintf(nomectn, "%s.ctn", nome);
    in = read_contour(nomectn, &nl, &nc);
    write_pbm(in, nomeimg, nl, nc);
    free(in);
    return 0;
}