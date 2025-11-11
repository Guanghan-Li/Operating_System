// Name: Guanghan Li
// Date: 2025-09-30
// Title: Lab2 – Step 5
// Description: This program turns step 3 into 4 processes.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int i, n[4];
    int proc_num = 0; // 0=parent, 1=child1, 2=child2, 3=child3

    if (argc != 5) {
        printf("Usage: %s <parent_delay> <child1_delay> <child2_delay> <child3_delay>\n", argv[0]);
        exit(0);
    }

    for (i = 0; i < 4; i++) {
        n[i] = atoi(argv[i + 1]);
    }
    printf("\n Before forking.\n");
    // First fork
    pid_t pid1 = fork();
    if (pid1 < 0) {
        fprintf(stderr, "can't fork, error %d\n", errno);
        exit(0);
    }
    if (pid1 == 0) {
        proc_num = 1; // child1
    }

    // Second fork
    pid_t pid2 = fork();
    if (pid2 < 0) {
        fprintf(stderr, "can't fork, error %d\n", errno);
        exit(0);
    }
    if (pid2 == 0) {
        if (proc_num == 1) proc_num = 3; // child1’s child
        else proc_num = 2; // parent’s child2
    }

    // Each process now knows its role (proc_num: 0=parent, 1=child1, 2=child2, 3=child3)
    for (i = 0; i < 10; i++) {
        switch(proc_num) {
            case 0:
                printf("\t\t\tI am the parent process displaying iteration %d\n", i);
                usleep(n[0]);
                break;
            case 1:
                printf("I am child 1 displaying iteration %d\n", i);
                usleep(n[1]);
                break;
            case 2:
                printf("I am child 2 displaying iteration %d\n", i);
                usleep(n[2]);
                break;
            case 3:
                printf("I am child 3 displaying iteration %d\n", i);
                usleep(n[3]);
                break;
        }
    }

    // Have parents wait for children (optional for clean exit)
    if (proc_num == 0 || proc_num == 1) {
        wait(NULL);
        wait(NULL);
    }

    return 0;
}
