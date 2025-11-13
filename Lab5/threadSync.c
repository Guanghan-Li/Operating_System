// Name: Guanghan Li
// Date: 2025-10-21
// Title: Lab5 – Step 1
// Description: This program uses threads to implement a critical section using semaphores. Compared to lab4, this program uses semaphores to implement a critical section. This mean we let all the threads to attempt at the same time, but due to the semaphore, only one thread can enter the critical section at a time. Instead of treads finishing randomly, now they will finish in order of the attempts.
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>


#define NTHREADS 10

void *go(void *arg);
void cleanup(int sigtype);

pthread_t threads[NTHREADS];
sem_t *mutex;

int main() {
    signal(SIGINT,cleanup);
    sem_unlink("/mutex");
    mutex = sem_open("/mutex", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
    if (mutex == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    int i;
    for (i = 0; i < NTHREADS; i++)  
        pthread_create(&threads[i], NULL, go, (void *)(size_t)i);
    for (i = 0; i < NTHREADS; i++) 
        pthread_join(threads[i],NULL);
    sem_close(mutex);
    sem_unlink("/mutex");
    return 0;
}

void *go(void *arg) {
    printf("Thread %d is now attempting ....\n",  (int)(size_t)arg);
    sem_wait(mutex);
    sleep(1);
    printf("Thread %d is running in its Critical Section........\n",  (int)(size_t)arg);
    sem_post(mutex);
    pthread_exit(0);
}

void cleanup(int sigtype){
    sem_close(mutex);
    sem_unlink("/mutex");
    printf("\n Terminating\n");
    exit(0);
}
