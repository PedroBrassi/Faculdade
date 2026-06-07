#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

/* =========================================================
   CONSTANTES DO PROGRAMA
========================================================= */

/* Quantidade de tamanhos diferentes */
#define NUM_SIZES 8

/* Quantidade de instâncias por tamanho */
#define INSTANCES_PER_SIZE 10

/* =========================================================
   FUNÇÃO: createDirectory
   Cria um diretório caso ele não exista.
========================================================= */
void createDirectory(const char *folderName) {

    struct stat st = {0};

    if (stat(folderName, &st) == -1) {
        mkdir(folderName, 0700);
    }
}

/* =========================================================
   FUNÇÃO: generateInstance
   Gera um arquivo de instância.

   Parâmetros:
   - filename: nome do arquivo
   - n: quantidade de elementos
   - minValue: valor mínimo
   - maxValue: valor máximo
========================================================= */
void generateInstance(const char *filename, int n, int minValue,int maxValue){

    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Erro em criar o arquivo: %s\n",filename);
        return;
    }

    /* Primeira linha = quantidade de números */
    fprintf(file, "%d\n", n);

    /* Geração dos números aleatórios */
    for (int i = 0; i < n; i++) {
        int value = rand() % (maxValue - minValue + 1)+ minValue;
        fprintf(file, "%d\n", value);
    }

    fclose(file);
}

/* =========================================================
   FUNÇÃO PRINCIPAL
========================================================= */
int main() {

    /* Inicializa gerador aleatório */
    srand(time(NULL));

    /* Tamanhos das instâncias */
    int sizes[NUM_SIZES] = {100,500,1000,5000,10000,100000,500000,1000000};

    char folderName[100];
    char fileName[200];

    /* Cria pasta principal */
    createDirectory("instances");

    /* Percorre todos os tamanhos */
    for (int i = 0; i < NUM_SIZES; i++) {

        /* Marca início da geração do grupo atual */
        clock_t start = clock();

        /* Nome da pasta */
        sprintf(folderName,"instances/instancia_%d",sizes[i]);

        /* Cria pasta específica */
        createDirectory(folderName);

        /* Cria as 10 instâncias */
        for (int j = 1;j <= INSTANCES_PER_SIZE;j++) {

            sprintf(fileName,"%s/instancia_%d_%d.txt",folderName,sizes[i],j);

            int minValue = 1;
            int maxValue = 10000;

            /* Instâncias grandes usam valores maiores */
            if (sizes[i] >= 100000) {
                maxValue = 1000000;
            }

            generateInstance(fileName,sizes[i],minValue,maxValue);
        }

        /* Marca final da geração */
        clock_t end = clock();

        double executionTime = ((double)(end - start))/ CLOCKS_PER_SEC;

        /* Exibe tempo total */
        printf( "%d instancias de %d criadas em %.2f secs\n",INSTANCES_PER_SIZE,sizes[i],executionTime);
    }

    printf(
        "\nTodas as intâncias foram criadas!\n"
    );

    return 0;
}