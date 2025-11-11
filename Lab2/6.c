// Name: Guanghan Li
// Date: 2025-09-30
// Title: Lab2 – Step 6
// Description: This program turns step 5 into 5 processes.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int n[5], i;
    int proc_num = 0;

    if (argc != 6) {
        printf("Usage: %s <parent_delay> <child1_delay> <child2_delay> <child3_delay> <child4_delay>\n", argv[0]);
        exit(1);
    }
    for (i = 0; i < 5; i++)
        n[i] = atoi(argv[i+1]);

    pid_t pid;
 
    printf("\n Before forking.\n");
    pid = fork();
    if (pid < 0) { perror("fork1"); exit(1); }
    if (pid == 0) {
        proc_num = 1; // In 1st child
        pid = fork();
        if (pid < 0) { perror("fork2"); exit(1); }
        if (pid == 0) {
            proc_num = 3; // In 3rd process (child of child1)
        }
    } else {
        pid = fork();
        if (pid < 0) { perror("fork3"); exit(1); }
        if (pid == 0) {
            proc_num = 2; // In 2nd child
        }
        if (pid > 0) {
            pid = fork();
            if (pid < 0) { perror("fork4"); exit(1); }
            if (pid == 0) {
                proc_num = 4; // In 4th child (individual)
            }
        }
    }

    // Display for each process
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
            case 4:
                printf("I am child 4 displaying iteration %d\n", i);
                usleep(n[4]);
                break;
        }
    }

    if (proc_num == 0 || proc_num == 1) {
        wait(NULL);
        wait(NULL);
    }


    return 0;
}
