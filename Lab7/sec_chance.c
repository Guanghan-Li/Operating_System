//Name: Guanghan Li
//Date: 2025-11-11
//Title: Lab7 - Step 3 - Second Chance Page Replacement Algorithm
//Description: This program implements the Second Chance page replacement algorithm.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
    int pageno;
    int ref_bit;  //If they recently got a second chance.  
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
    int clock_hand = 0;

    for (int i = 0; i < cacheSize; i++) {
        cache[i].pageno = -1;
        cache[i].ref_bit = 0;
    }

    while (fgets(pageCache, sizeof(pageCache), stdin)) {
        char *endptr;
        int page_num = (int)strtol(pageCache, &endptr, 10);
        if (endptr == pageCache) {
            continue;
        }

        bool hit = false;

        for (int i = 0; i < cacheSize; i++) {
            if (cache[i].pageno == page_num) {//If we hit then set ref_bit
                hit = true;
                cache[i].ref_bit = 1;  
                break;
            }
        }

        if (!hit) {// IF we miss then we check if empty or not and if not then we check if we can give a second chance.
            printf("%d\n", page_num);
            totalFaults++;

            int empty_index = -1;
            for (int i = 0; i < cacheSize; i++) {
                if (cache[i].pageno == -1) {
                    empty_index = i;
                    break;
                }
            }

            if (empty_index != -1) {
                cache[empty_index].pageno = page_num;
                cache[empty_index].ref_bit = 1;
            } else {
                while (1) {//This is the second chance algorithm.
                    if (cache[clock_hand].ref_bit == 0) {
                        cache[clock_hand].pageno = page_num;
                        cache[clock_hand].ref_bit = 1;
                        clock_hand = (clock_hand + 1) % cacheSize;
                        break;
                    } else {
                        cache[clock_hand].ref_bit = 0;
                        clock_hand = (clock_hand + 1) % cacheSize;
                    }
                }
            }
        }
    }
    return 0;
}
