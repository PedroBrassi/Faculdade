#include <stdio.h>
#include <stdlib.h>

#include "leitor_instancias.h"

/* =========================================================
   FUNÇÃO: readInstance

   Lê uma instância do arquivo.

   Formato esperado:

   n
   valor1
   valor2
   valor3
   ...

   Parâmetros:
   - fileName: nome do arquivo
   - n: retorna quantidade de elementos

   Retorno:
   - vetor alocado dinamicamente
========================================================= */

int *readInstance(const char *fileName, int *n) {

    FILE *file = fopen(fileName, "r");

    /* Verifica erro ao abrir */
    if (file == NULL) {

        printf("Erro em abrir o arquivo: %s\n",fileName);

        return NULL;
    }

    /* Lê quantidade de elementos */
    if (fscanf(file, "%d", n) != 1) {
        printf("Erro ao ler tamanho da instancia.\n");
        fclose(file);
        return NULL;
    }

    /* Alocalçao dinâmica de memória */
    int *values = (int *) malloc((*n) * sizeof(int));

    if (values == NULL) {

        printf("Erro em alocar memória.\n");

        fclose(file);

        return NULL;
    }

    /* Leitura dos valores */
    for (int i = 0; i < *n; i++) {

        if (fscanf(file,"%d",&values[i]) != 1) {

            printf("Erro ao ler o valor: %d.\n",i);

            free(values);
            fclose(file);

            return NULL;
        }
    }

    fclose(file);

    return values;
}