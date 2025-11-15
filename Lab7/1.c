//Name: Guanghan Li
//Date: 2025-11-11
//Title: Lab7 - Step 1 - Generate Test Input
//Description: This program generates a test input file for the page replacement algorithms.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define numRequests 300
#define maxPageNumber 20

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[20];           
    int i;

    fp = fopen("testInput.txt", "w");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    srand((unsigned int)time(NULL));   

    for (i = 0; i < numRequests; i++) {
        sprintf(buffer, "%d\n", rand() % maxPageNumber);
        fputs(buffer, fp);
    }

    fclose(fp);
    return 0;
}
