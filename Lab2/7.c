// Name: Guanghan Li
// Date: 2025-09-30
// Title: Lab2 – Step 7
// Description: This program uses execlp to run the ls command.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main() {
    pid_t pid;

    printf("Before forking.\n");
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "can't fork, error %d\n", errno);
        exit(1);
    }
    if (pid == 0) {
        // Child process: run "ls" using execlp
        execlp("/bin/ls", "ls", (char *)0);
    } else {
        // Parent process: wait for child to finish
        wait(0);
        printf("Child has completed the task! \n");
        exit(0);
    }
}
