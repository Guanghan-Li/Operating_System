//Name: Guanghan Li
//Date: 2025-11-11
//Title: Lab7 - Step 3 - FIFO Page Replacement Algorithm
//Description: This program implements the FIFO page replacement algorithm.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
    int pageno;
} ref_page;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <cacheSize>\n", argv[0]);
        return 1;
    }

    int cacheSize = atoi(argv[1]);
    if (cacheSize <= 0) {
        fprintf(stderr, "cacheSize must be > 0\n");
        return 1;
    }

    ref_page cache[cacheSize];
    char pageCache[100];
    int totalFaults = 0;

    for (int i = 0; i < cacheSize; i++) {
        cache[i].pageno = -1;
    }

    int next_to_replace = 0;  

    while (fgets(pageCache, sizeof(pageCache), stdin)) {
        char *endptr;
        int page_num = (int)strtol(pageCache, &endptr, 10);
        if (endptr == pageCache) {
            continue;  
        }

        bool foundInCache = false;

        for (int i = 0; i < cacheSize; i++) {
            if (cache[i].pageno == page_num) {
                foundInCache = true;
                break;
            }
        }

        if (!foundInCache) {
            printf("%d\n", page_num);
            totalFaults++;

            bool placed = false;
            for (int i = 0; i < cacheSize; i++) {
                if (cache[i].pageno == -1) {
                    cache[i].pageno = page_num;
                    placed = true;
                    break;
                }
            }
            //FIFO
            if (!placed) {
                cache[next_to_replace].pageno = page_num;
                next_to_replace = (next_to_replace + 1) % cacheSize;
            }
        }
    }
    return 0;
}
