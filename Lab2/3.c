// Name: Guanghan Li
// Date: 2025-09-30
// Title: Lab2 – Step 3 - Step 4
// Description: This program displays the parent and child process displaying the iteration number with a delay of the user's given delay.

#include <stdio.h>      /* printf, stderr */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* fork, usleep */
#include <stdlib.h>     /* atoi, exit */
#include <errno.h>      /* errno */

/* main function with command-line arguments */
int main(int argc, char *argv[]) {
    pid_t pid;
    int i, n;

    // Check for correct number of arguments
    if (argc != 2) {
        printf("Usage: %s <delay>\n", argv[0]);
        exit(0);
    }

    // Convert argument to integer
    n = atoi(argv[1]);

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
        wait(NULL);
    } else {
        // Child process: pid = 0
        for (i = 0; i < 10; i++) {
            printf("I am the child process displaying iteration %d\n", i);
            usleep(n);
        }
    }
    return 0;
}
