//Name: Guanghan Li
//Date: 2025-10-07
//Title: Lab3 – Step 4 - cat | grep
//Description: This program shows pipes for IPC using two children. The parent create a pipe and then fork it twice. The first child runs cat and the second child runs grep. The output is the lines that contain the word "root".
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fds[2];
    pipe(fds);

    // child 1: stdout -> pipe write end; run cat /etc/passwd
    if (fork() == 0) {
        dup2(fds[1], 1);
        close(fds[0]);
        close(fds[1]);
        execlp("cat", "cat", "/etc/passwd", 0);
    }
    // child 2: stdin <- pipe read end; run grep root
    else if (fork() == 0) {
        dup2(fds[0], 0);
        close(fds[1]);
        close(fds[0]);
        execlp("grep", "grep", "root", 0);
    }

    // parent
    close(fds[0]);
    close(fds[1]);
    wait(0);
    wait(0);
    return 0;
}
