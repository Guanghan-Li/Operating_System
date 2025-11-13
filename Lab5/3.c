// Name: Guanghan Li
// Date: 2025-10-21
// Title: Lab5 – Step 3
// Description: This program uses threads to implement a producer-consumer problem using named semaphores.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 9
#define TOTAL_ITEMS 20

static const char *SEM_MUTEX = "/lab5_mutex3";
static const char *SEM_EMPTY = "/lab5_empty3";
static const char *SEM_FULL  = "/lab5_full3";

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t *mutex;
sem_t *empty;
sem_t *full;

void *producer(void *arg);
void *consumer(void *arg);

int main(void) {
    pthread_t prod_tid;
    pthread_t cons_tid;

    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);

    mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
    empty = sem_open(SEM_EMPTY, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, BUFFER_SIZE);
    full  = sem_open(SEM_FULL,  O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);

    if (mutex == SEM_FAILED || empty == SEM_FAILED || full == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid, NULL, consumer, NULL);

    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    sem_close(mutex);
    sem_close(empty);
    sem_close(full);
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);

    return 0;
}

void *producer(void *arg) {
    (void)arg;
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        int item = i + 1;
        sem_wait(empty);
        sem_wait(mutex);

        buffer[in] = item;
        printf("Producer: placed item %d at %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(full);
    }
    return NULL;
}

void *consumer(void *arg) {
    (void)arg;
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        sem_wait(full);
        sem_wait(mutex);

        int item = buffer[out];
        printf("Consumer: removed item %d from %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(empty);
    }
    return NULL;
}
