//Name: Guanghan Li
//Date: 2025-10-07
//Title: Lab3 – Step 6 - Shared Memory
//Description: This program uses shared memory to pass a message between a parent and a child, the parent writes a message to the shared memory and the child reads the message from the shared memory.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>   
#include <sys/ipc.h>     
#include <sys/shm.h>
#include <sys/wait.h>

#define MSIZ 100

struct info {
    char buf[MSIZ];
};

int main(void) {
    key_t key = 1234;
    int id = shmget(key, MSIZ, IPC_CREAT | 0666);
    struct info *ctrl = (struct info *) shmat( id, 0, 0);

    if (fork() == 0) {
        int cid = shmget(key, MSIZ, 0);
        struct info *cptr = (struct info *)shmat(cid, 0, 0);
        printf("%s", cptr->buf);
        shmdt(cptr);
        return 0;
    }

    // parent: writer
    const char *msg = "This is the message from the parent.\n";
    int i = 0;
    while (msg[i] && i < MSIZ - 1) { 
        ctrl->buf[i] = msg[i]; i++; 
    }
    ctrl->buf[i] = '\0';

    wait(0);
    shmdt(ctrl);
    shmctl(id, IPC_RMID, 0);
    return 0;
}