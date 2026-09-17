#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define REPETICOES 1000000

long contador = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *incrementar(void *arg) {
    (void)arg;
    for (long i = 0; i < REPETICOES; i++) {
        pthread_mutex_lock(&mutex);
        contador++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, incrementar, NULL);
    pthread_create(&t2, NULL, incrementar, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Esperado: %d\n", 2 * REPETICOES);
    printf("Obtido: %ld\n", contador);

    pthread_mutex_destroy(&mutex);
    return EXIT_SUCCESS;
}