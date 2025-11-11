//Name: Guanghan Li
//Date: 2025-10-07
//Title: Lab3 – Step 5 - Pipe User Input
//Description: This program shows pipes for IPC using two children. The parent create a pipe and then fork it twice. The first child reads a sentence from the user and sends it to the second child. The second child prints the sentence.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fds[2];
    char buff[100];
    int count;

    pipe(fds);

    if (fork() == 0) {
        // Producer: read a sentence from the user and send it
        close(fds[0]);

        printf("Producer: enter a sentence: ");
        if (!fgets(buff, sizeof(buff), stdin))
            exit(0);

        for (int i = 0; buff[i] != '\0'; i++)
            write(fds[1], &buff[i], 1);

        close(fds[1]);
        exit(0);
    } else {
        // Consumer: print what arrives from the pipe
        close(fds[1]);
        printf("Consumer: receiving message...\n");

        while ((count = read(fds[0], buff, sizeof(buff))) > 0)
            write(1, buff, count);
        write(1, "\n", 1);
        close(fds[0]);
        wait(0);
    }
    return 0;
}
