//Name: Guanghan Li
//Date: 2025-10-07
//Title: Lab3 – Step 7 - Threads
//Description: This program uses threads to display the iteration number. The bug here is that the threads all get the same pointer &i so it will print what ever i is holding at that moment. This case it could be anything from 0 to 9 while the loop is running, many threads can read i at the same time therefor I observed some duplicates for example i saw 3 of them with iteration 1 and 2 of them had iteration 0.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *go(void *);

#define NTHREADS 10

pthread_t threads[NTHREADS];

int main() {
    int i;
    for (i = 0; i < NTHREADS; i++)
        pthread_create(&threads[i], NULL, go, &i);
    for (i = 0; i < NTHREADS; i++) {
        printf("Thread %d returned\n", i);
        pthread_join(threads[i],NULL);
    }
    printf("Main thread done.\n");
    return 0;
}

void *go(void *arg) {
    printf("Hello from thread %d with iteration %d\n", (int)(unsigned long) pthread_self(), *(int *)arg);
    return 0;
}