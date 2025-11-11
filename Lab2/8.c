// Name: Guanghan Li
// Date: 2025-09-30
// Title: Lab2 – Step 8
// Description: This program uses threads to display the iteration number with a delay of the user's given delay.
#include <stdio.h>       
#include <stdlib.h>      
#include <unistd.h>      
#include <pthread.h>     

void *parent_thread_func(void *arg) {
    int *delay_ptr = (int *)arg;
    int delay = *delay_ptr;
    for (int i = 0; i < 10; i++) {
        printf("\t \t \t I am the parent Thread displaying iteration %d\n", i);
        usleep(delay);
    }
    return NULL;
}

void *child_thread_func(void *arg) {
    int *delay_ptr = (int *)arg;
    int delay = *delay_ptr;
    for (int i = 0; i < 10; i++) {
        printf("I am the child Thread displaying iteration %d\n", i);
        usleep(delay);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t parent_thread, child_thread;
    int n;

    // Check for correct number of arguments
    if (argc != 2) {
        printf("Usage: %s <delay>\n", argv[0]);
        exit(0);
    }

    n = atoi(argv[1]);
    printf("\n Before threading.\n");

    // Create parent thread
    if (pthread_create(&parent_thread, NULL, parent_thread_func, &n) != 0) {
        perror("Failed to create parent thread");
        exit(1);
    }
    // Create child thread
    if (pthread_create(&child_thread, NULL, child_thread_func, &n) != 0) {
        perror("Failed to create child thread");
        exit(1);
    }

    // Wait for both threads to finish
    pthread_join(parent_thread, NULL);
    pthread_join(child_thread, NULL);

    return 0;
}
