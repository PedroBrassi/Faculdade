#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#define REPETICOES 1000000

long contador = 0;
atomic_int interesse[2];
atomic_int vez;

void entrar_regiao_critica(int id) {
    int outro = 1 - id;
    atomic_store(&interesse[id], 1);
    atomic_store(&vez, outro);
    while (atomic_load(&interesse[outro]) && atomic_load(&vez) == outro) {
        /* espera ocupada (busy waiting) */
    }
}

void sair_regiao_critica(int id) {
    atomic_store(&interesse[id], 0);
}

void *incrementar(void *arg) {
    int id = *(int *)arg;
    for (long i = 0; i < REPETICOES; i++) {
        entrar_regiao_critica(id);
        contador++;
        sair_regiao_critica(id);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    int id0 = 0, id1 = 1;

    atomic_init(&interesse[0], 0);
    atomic_init(&interesse[1], 0);
    atomic_init(&vez, 0);

    pthread_create(&t1, NULL, incrementar, &id0);
    pthread_create(&t2, NULL, incrementar, &id1);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Esperado: %d\n", 2 * REPETICOES);
    printf("Obtido: %ld\n", contador);

    return EXIT_SUCCESS;
}