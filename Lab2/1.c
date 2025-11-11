// Name: Guanghan Li
// Date: 2025-09-30
// Title: Lab2 – Step 1 - Step 2
// Description: This program displays the parent and child process displaying the iteration number with a delay of 3000 microseconds.

#include <stdio.h>      /* printf, stderr */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* fork */
#include <stdlib.h>     /* atoi */
#include <errno.h>      /* errno */

/* main function */
int main() {
    pid_t pid;
    int i, n = 3000; // n is a delay in microseconds inserted in parent and child iterations
    
    printf("\n Before forking.\n");
    pid = fork();
    
    if (pid < 0) {
        fprintf(stderr, "can't fork, error %d\n", errno);
        exit(0);
    }
    
    if (pid) {
        // Parent process: pid is > 0
        for (i = 0; i < 10; i++) {
            printf("\t \t \t I am the parent Process displaying iteration %d \n", i);
            usleep(n);
        }
        wait(0);
    }
    else {
        // Child process: pid = 0
        for (i = 0; i < 10; i++) {
            printf("I am the child process displaying iteration %d\n", i);
            usleep(n);
        }
    }
    return 0;
}
