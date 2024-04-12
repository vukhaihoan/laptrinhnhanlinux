#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem1, sem2, sem3;

void *threadFunc1(void *arg) {
    sem_wait(&sem1);
    printf("R ");
    sem_post(&sem2);
    sem_wait(&sem3);
    printf("OK1 \n");
    sem_post(&sem1);
    return NULL;
}

void *threadFunc2(void *arg) {
    sem_wait(&sem2);
    printf("I ");
    sem_post(&sem3);
    sem_wait(&sem2);
    printf("OK2 ");
    sem_post(&sem3);
    return NULL;
}

void *threadFunc3(void *arg) {
    sem_wait(&sem3);
    printf("O ");
    sem_post(&sem1);
    sem_wait(&sem1);
    printf("OK3 ");
    sem_post(&sem2);
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);
    sem_init(&sem3, 0, 0);

    pthread_create(&t1, NULL, threadFunc1, NULL);
    pthread_create(&t2, NULL, threadFunc2, NULL);
    pthread_create(&t3, NULL, threadFunc3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);

    return 0;
}

