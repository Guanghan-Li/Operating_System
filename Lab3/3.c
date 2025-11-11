//Name: Guanghan Li
//Date: 2025-10-07
//Title: Lab3 – Step 3 - ls | grep
//Description: This program shows pipes for IPC using two children. The parent create a pipe and then fork it twice. The first child runs ls and the second child runs grep. The output is the files that contain the word "ls".
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// main
int main(int argc, char *argv[]) {
    int fds[2];
    char buff[60];
    int count;
    int i;

    pipe(fds);
    if (fork()==0){
        // Writer: redirect stdout to pipe write-end, then exec ls
        printf("\nWriter on the upstream end of the pipe -> running ls\n");
        close(fds[0]);
        dup2(fds[1], 1);              
        close(fds[1]);                
        execlp("ls", "ls", 0);
        exit(1);
    }
    else if(fork()==0){
        // Reader: same as your base code
        printf("\nReader on the downstream end of the pipe \n");
        close(fds[1]);
        while((count = read(fds[0], buff, 60)) > 0){
            for(i = 0; i < count; i++){
                write(1, buff + i, 1);
                write(1, " ", 1);
            }
            printf("\n");
        }
        close(fds[0]);
        exit(0);
    }
    else{
        close(fds[0]);
        close(fds[1]);
        wait(0);
        wait(0);
    }
    return 0;
}
