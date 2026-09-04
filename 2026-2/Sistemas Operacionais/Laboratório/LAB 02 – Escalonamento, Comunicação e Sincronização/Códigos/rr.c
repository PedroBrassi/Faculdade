#include <stdio.h>
#define N 4

int main(void) {
    int chegada[N] = {0, 1, 2, 3};
    int burst[N]   = {8, 2, 1, 3};
    int restante[N];
    int fim[N] = {0};
    int quantum = 2;
    int tempo = 0;
    int concluidos = 0;

    for (int i = 0; i < N; i++)
        restante[i] = burst[i];

    printf("Ordem de uso da CPU:\n");
    while (concluidos < N) {
        int executou = 0;
        for (int i = 0; i < N; i++) {
            if (chegada[i] <= tempo && restante[i] > 0) {
                int fatia = restante[i] < quantum ? restante[i] : quantum;
                printf("t=%d -> P%d por %d unidade(s)\n", tempo, i + 1, fatia);
                tempo += fatia;
                restante[i] -= fatia;
                executou = 1;

                if (restante[i] == 0) {
                    fim[i] = tempo;
                    concluidos++;
                }
            }
        }
        if (!executou)
            tempo++;
    }

    printf("\nP  Fim  Turnaround  Espera\n");
    for (int i = 0; i < N; i++) {
        int turnaround = fim[i] - chegada[i];
        int espera = turnaround - burst[i];
        printf("P%d %4d %11d %6d\n", i + 1, fim[i], turnaround, espera);
    }

    return 0;
}