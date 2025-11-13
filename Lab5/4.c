// Name: Guanghan Li
// Date: 2025-10-21
// Title: Lab5 – Step 4
// Description: This program uses threads to implement a producer-consumer problem using mutex and condition variables. This does now have waiting.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 20

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg);
void *consumer(void *arg);

int main(void) {
    pthread_t prod_tid;
    pthread_t cons_tid;

    if (pthread_create(&prod_tid, NULL, producer, NULL) != 0) {
        perror("pthread_create producer");
        return EXIT_FAILURE;
    }

    if (pthread_create(&cons_tid, NULL, consumer, NULL) != 0) {
        perror("pthread_create consumer");
        return EXIT_FAILURE;
    }

    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}

void *producer(void *arg) {
    (void)arg;
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        int item = i + 1;

        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &mutex);
        }

        buffer[in] = item;
        printf("Producer placed %d at %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    (void)arg;
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0) {
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = buffer[out];
        printf("Consumer removed %d from %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
