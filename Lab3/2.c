//Name: Guanghan Li
//Date: 2025-10-07
//Title: Lab3 – Step 2 - Pipe Arguments
//Description: This program shows pipes for IPC using two children. The parent create a pipe and then fork it twice. The first child writes the arguments to the pipe and the second child reads the arguments from the pipe. The output is the input with a space in between each character.

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
        printf("\nWriter on the upstream end of the pipe -> %d arguments \n",argc);
        close(fds[0]);
        for(i=0;i<argc;i++){
            write(fds[1],argv[i],strlen(argv[i]));
        }
        exit(0);
    }
    else if(fork()==0){
        printf("\nReader on the downstream end of the pipe \n");
        close(fds[1]);
        while((count=read(fds[0],buff,60))>0){
            for(i=0;i<count;i++){
                write(1,buff+i,1);
                write(1," ",1);
            }
            printf("\n");
        }
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
