//Name: Guanghan Li
//Date: 2025-10-07
//Title: Lab3 – Step 8 - Threads
//Description: This program uses threads to display the iteration number. The bug is fixed here because all of the threads get their own id. So non of the threads will get the same id at the same time.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NTHREADS 10

void *go(void *arg) {
    int id = *(int *)arg;                       // each thread sees its own id
    printf("Hello from thread %d with iteration %d\n", (int)(unsigned long) pthread_self(), id);
    return NULL;
}

int main(void) {
    pthread_t threads[NTHREADS];
    int ids[NTHREADS];

    for (int i = 0; i < NTHREADS; i++) {
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, go, &ids[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d returned\n", i);
    }

    printf("Main thread done.\n");
    return 0;
}
