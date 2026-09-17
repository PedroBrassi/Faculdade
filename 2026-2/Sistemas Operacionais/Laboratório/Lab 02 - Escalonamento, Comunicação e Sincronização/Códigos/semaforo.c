#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define ALUNOS 8

sem_t computadores;

void *usar_computador(void *arg) {
    long id = *(long *)arg;
    printf("Aluno %ld chegou.\n", id);
    
    sem_wait(&computadores);
    printf("Aluno %ld CONSEGUIU um computador.\n", id);
    sleep(2);
    printf("Aluno %ld LIBEROU o computador.\n", id);
    sem_post(&computadores);
    
    return NULL;
}

int main(void) {
    pthread_t threads[ALUNOS];
    long ids[ALUNOS];

    sem_init(&computadores, 0, 3); /* Inicializa com 3 vagas */

    for (long i = 0; i < ALUNOS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, usar_computador, &ids[i]);
    }

    for (int i = 0; i < ALUNOS; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&computadores);
    return EXIT_SUCCESS;
}