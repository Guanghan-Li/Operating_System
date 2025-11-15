//Name: Guanghan Li
//Date: 2025-11-11
//Title: Lab7 - Step 3 - LRU Page Replacement Algorithm
//Description: This program implements the LRU page replacement algorithm.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
    int pageno;
    int last_used;   //This is the history.
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
    int time = 0;

    for (int i = 0; i < cacheSize; i++) {
        cache[i].pageno = -1;
        cache[i].last_used = 0;
    }

    while (fgets(pageCache, sizeof(pageCache), stdin)) {
        char *endptr;
        int page_num = (int)strtol(pageCache, &endptr, 10);
        if (endptr == pageCache) {
            continue;
        }

        time++;

        bool hit = false;
        int hit_index = -1;

        for (int i = 0; i < cacheSize; i++) {
            if (cache[i].pageno == page_num) {
                hit = true;
                hit_index = i;
                break;
            }
        }

        if (hit) {
            cache[hit_index].last_used = time; //update the history.
        } else {
            printf("%d\n", page_num);
            totalFaults++;

            int empty_index = -1;
            for (int i = 0; i < cacheSize; i++) {
                if (cache[i].pageno == -1) {
                    empty_index = i;
                    break;
                }
            }
            //This check if it's empty or not.
            if (empty_index != -1) {
                cache[empty_index].pageno = page_num;
                cache[empty_index].last_used = time;
            } else {
                int lru_index = 0;
                int min_time = cache[0].last_used;

                for (int i = 1; i < cacheSize; i++) {
                    if (cache[i].last_used < min_time) {
                        min_time = cache[i].last_used;
                        lru_index = i;
                    }
                }

                cache[lru_index].pageno = page_num;
                cache[lru_index].last_used = time;
            }
        }
    }
    return 0;
}
