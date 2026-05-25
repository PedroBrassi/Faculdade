
//Programa principal para ordenar jogadores a partir de jogadores.csv

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "leitura.h"
#include "ordenacao.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso correto: ./jogadores <1|2|3>\n");
        return 1;
    }

    int metodo = atoi(argv[1]);
    if (metodo < 1 || metodo > 3) {
        printf("Método inválido! Use 1, 2 ou 3.\n");
        return 1;
    }

    long long comp = 0, troca = 0, mem = 0;
    int qtd = 0;

    // lê arquivo e atualiza mem (mem começa em 0)
    Jogador *lista = lerArquivoCSV("jogadores.csv", &qtd, &mem);
    if (!lista) {
        fprintf(stderr, "Erro ao ler jogadores.csv\n");
        return 1;
    }

    clock_t ini = clock();

    if (metodo == 1) {
        selectionSort(lista, qtd, &comp, &troca, &mem);
    } else if (metodo == 2) {
        mergeSort(lista, 0, qtd - 1, &comp, &troca, &mem);
    } else { // metodo == 3
        radixSortNomes(lista, qtd, &comp, &troca, &mem);
    }

    clock_t fim = clock();

    // imprimir no formato CSV (com cabeçalho)
    imprimirJogadoresCSV(lista, qtd);

    double tempo = (double)(fim - ini) / CLOCKS_PER_SEC * 1000.0; // ms

    // impressões finais (cada em sua linha) conforme enunciado
    printf("%.2f\n", tempo);
    printf("%lld\n", comp);
    printf("%lld\n", troca);
    printf("%lld\n", mem);

    free(lista);
    return 0;
}
