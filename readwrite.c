#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t resource, rmutex;
int read_count = 0;

void *reader(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sem_wait(&rmutex);
        read_count++;
        if (read_count == 1) sem_wait(&resource);
        sem_post(&rmutex);

        printf("Reader %d is reading\n", id);
        sleep(1);

        sem_wait(&rmutex);
        read_count--;
        if (read_count == 0) sem_post(&resource);
        sem_post(&rmutex);
        sleep(1);
    }
}

void *writer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sem_wait(&resource);

        printf("Writer %d is writing\n", id);
        sleep(1);

        sem_post(&resource);
        sleep(1);
    }
}

int main() {
    pthread_t rtid[5], wtid[2];
    int ids[5];
    sem_init(&resource, 0, 1);
    sem_init(&rmutex, 0, 1);

    for (int i = 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&rtid[i], NULL, reader, &ids[i]);
        if (i < 2) pthread_create(&wtid[i], NULL, writer, &ids[i]);
    }

    for (int i = 0; i < 5; i++) pthread_join(rtid[i], NULL);
    for (int i = 0; i < 2; i++) pthread_join(wtid[i], NULL);

    sem_destroy(&resource);
    sem_destroy(&rmutex);
    return 0;
}
