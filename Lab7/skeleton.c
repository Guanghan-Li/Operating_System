#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int pageno;
    // you'll add more fields here later for LRU / Second Chance
} ref_page;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <cacheSize>\n", argv[0]);
        return 1;
    }

    int cacheSize = atoi(argv[1]);      // Size of cache passed by user
    if (cacheSize <= 0) {
        fprintf(stderr, "cacheSize must be > 0\n");
        return 1;
    }

    ref_page cache[cacheSize];          // Cache that stores pages
    char pageCache[100];                // Buffer for each input line
    int totalFaults = 0;                // Total page faults

    // Initialize cache array to "empty"
    for (int i = 0; i < cacheSize; i++) {
        cache[i].pageno = -1;
    }

    // Read page requests from stdin until EOF
    while (fgets(pageCache, sizeof(pageCache), stdin)) {
        // Convert start of line to an int
        char *endptr;
        int page_num = (int)strtol(pageCache, &endptr, 10);

        // If line doesn't start with a number, ignore it
        if (endptr == pageCache) {
            continue;
        }
    }

    // If you want to see total faults while debugging:
    // fprintf(stderr, "%d Total Page Faults\n", totalFaults);

    return 0;
}
