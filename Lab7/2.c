//Name: Guanghan Li
//Date: 2025-11-11
//Title: Lab7 - Step 2 - Page Replacement Algorithms
//Description: This program implements the page replacement algorithms a skeleton.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    while (fgets(pageCache, sizeof(pageCache), stdin)) {
        char *endptr;
        int page_num = (int)strtol(pageCache, &endptr, 10);

        if (endptr == pageCache) {
            continue;
        }


    }

    return 0;
}
