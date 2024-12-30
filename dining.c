#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5

sem_t forks[N];
pthread_mutex_t lock;

void *philosopher(void *arg) {
    int id = *(int *)arg;

    while (1) {
        printf("Philosopher %d is thinking\n", id);
        sleep(1);

        pthread_mutex_lock(&lock);
        sem_wait(&forks[id]);
        sem_wait(&forks[(id + 1) % N]);
        pthread_mutex_unlock(&lock);

        printf("Philosopher %d is eating\n", id);
        sleep(1);

        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % N]);
    }
}

int main() {
    pthread_t tid[N];
    int ids[N];

    pthread_mutex_init(&lock, NULL);
    for (int i = 0; i < N; i++) sem_init(&forks[i], 0, 1);

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&lock);
    for (int i = 0; i < N; i++) sem_destroy(&forks[i]);

    return 0;
}
