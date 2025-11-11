//Name: Guanghan Li
//Date: 2025-10-07
//Title: Lab3 – Step 1 - Run Command
//Description: This program runs the more command and the ls command it uses pipe to connect the two commands.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// main function
int main() {
    int fds[2];
    pipe(fds);

    // child 1: redirects stdin to downstream of the pipe, then exec 'more'
    if (fork() == 0) {
        dup2(fds[0], 0);      
        close(fds[1]); 
        execlp("more", "more", 0);
    }
    // child 2: redirects stdout to upstream of the pipe, then exec 'ls'
    else if (fork() == 0) {
        dup2(fds[1], 1);
        close(fds[0]); 
        execlp("ls", "ls", "-l", 0);
    }
    else {
        // parent closes both ends and waits for children
        close(fds[0]);
        close(fds[1]);
        wait(0);
        wait(0);
    }

    return 0;
}
