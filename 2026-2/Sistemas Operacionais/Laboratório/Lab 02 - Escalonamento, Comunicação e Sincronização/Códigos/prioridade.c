#include <stdio.h>
#define N 4

int main(void) {
    int chegada[N]    = {0, 0, 0, 0};
    int burst[N]      = {6, 3, 2, 4};
    int prioridade[N] = {3, 1, 0, 2}; /* menor numero = maior prioridade */
    int usado[N]      = {0};
    int tempo         = 0;

    printf("Ordem escolhida por prioridade:\n");
    for (int passo = 0; passo < N; passo++) {
        int escolhido = -1;
        for (int i = 0; i < N; i++) {
            if (!usado[i] && chegada[i] <= tempo) {
                if (escolhido == -1 || prioridade[i] < prioridade[escolhido]) {
                    escolhido = i;
                }
            }
        }
        if (escolhido == -1) {
            tempo++;
            passo--;
            continue;
        }

        printf("t=%d -> P%d | prioridade=%d | burst=%d\n",
               tempo, escolhido + 1,
               prioridade[escolhido], burst[escolhido]);
        tempo += burst[escolhido];
        usado[escolhido] = 1;
    }

    return 0;
}