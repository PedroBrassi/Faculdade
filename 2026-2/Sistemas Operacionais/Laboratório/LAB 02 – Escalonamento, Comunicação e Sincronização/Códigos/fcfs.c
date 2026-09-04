#include <stdio.h>
#define N 4

int main(void) {
    int chegada[N] = {0, 1, 2, 3};
    int burst[N]   = {8, 2, 1, 3};
    int inicio[N], fim[N], espera[N], turnaround[N];
    int tempo = 0;

    for (int i = 0; i < N; i++) {
        if (tempo < chegada[i])
            tempo = chegada[i];
        
        inicio[i] = tempo;
        espera[i] = inicio[i] - chegada[i];
        tempo += burst[i];
        fim[i] = tempo;
        turnaround[i] = fim[i] - chegada[i];
    }

    printf("P  Chegada  Burst  Inicio  Fim  Espera  Turnaround\n");
    for (int i = 0; i < N; i++) {
        printf("P%d %7d %6d %7d %4d %7d %11d\n",
               i + 1, chegada[i], burst[i], inicio[i],
               fim[i], espera[i], turnaround[i]);
    }

    return 0;
}