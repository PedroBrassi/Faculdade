#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int item_disponivel = 0;
int item = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *consumidor(void *arg) {
    (void)arg;
    pthread_mutex_lock(&mutex);
    while (!item_disponivel) {
        printf("Consumidor: nada pronto. Vou dormir...\n");
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Consumidor recebeu item %d.\n", item);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *produtor(void *arg) {
    (void)arg;
    sleep(2);
    pthread_mutex_lock(&mutex);
    item = 42;
    item_disponivel = 1;
    printf("Produtor: item pronto!\n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void) {
    pthread_t prod, cons;
    pthread_create(&cons, NULL, consumidor, NULL);
    pthread_create(&prod, NULL, produtor, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    return EXIT_SUCCESS;
}