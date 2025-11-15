# Lab 7 – Page Replacement Experiments

## Overview
This lab explores virtual memory management through simulations of classic page replacement algorithms. You'll implement FIFO (First-In-First-Out), LRU (Least Recently Used), and Second Chance algorithms, then compare their performance on various memory reference patterns. Understanding page replacement is crucial for comprehending how operating systems manage limited physical memory and optimize for different workload patterns.

## Purpose
Simulate classic page-replacement strategies so I can compare FIFO, LRU, and Second Chance behavior on the same memory-access traces.

## Prerequisites
- C compiler (gcc or clang)
- Basic understanding of virtual memory concepts
- Knowledge of data structures (queues, arrays)
- Familiarity with file I/O and stdin/stdout
- Python (optional, for visualization)

## Core Concepts

### Virtual Memory and Paging
- **Virtual Memory**: Abstraction that gives each process its own address space
- **Page**: Fixed-size block of virtual memory (typically 4KB)
- **Frame**: Fixed-size block of physical memory
- **Page Fault**: Exception when accessing page not in physical memory
- **Page Replacement**: Algorithm to decide which page to evict when physical memory is full

### Page Replacement Algorithms

#### 1. FIFO (First-In-First-Out)
**Strategy**: Evict the oldest page in memory (first one loaded).

**Pros**:
- Simple to implement (queue data structure)
- Low overhead
- Predictable behavior

**Cons**:
- Suffers from Belady's Anomaly (more frames can mean more faults!)
- May evict frequently-used pages
- No consideration of access patterns

**Implementation**: Circular queue or index pointer

#### 2. LRU (Least Recently Used)
**Strategy**: Evict the page that hasn't been used for the longest time.

**Pros**:
- Excellent performance on real workloads
- Respects locality of reference
- No Belady's Anomaly

**Cons**:
- Higher overhead (tracking timestamps/counters)
- More complex implementation
- Expensive in hardware (requires updates on every access)

**Implementation**: Timestamp tracking or counter

#### 3. Second Chance (Clock Algorithm)
**Strategy**: FIFO with a "second chance" via reference bit.

**Pros**:
- Compromise between FIFO and LRU
- Low overhead (single bit per page)
- Better than pure FIFO
- Hardware-friendly

**Cons**:
- Still not as good as LRU
- Worst case degrades to FIFO
- Requires reference bit support

**Implementation**: Circular list with reference bits

### Belady's Anomaly
**Surprising phenomenon**: For FIFO, increasing cache size can sometimes increase page faults!

**Example**:
```
Reference string: 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5

3 frames (FIFO): 9 faults
4 frames (FIFO): 10 faults  ← More frames, MORE faults!
```

This doesn't happen with LRU or optimal replacement.

## Files in This Lab

### `1.c` - Reference String Generator
**Purpose**: Generate test input for quick validation and experimentation.

**What it does**:
- Creates `testInput.txt` with random page references
- Configurable parameters: number of accesses, page range
- Useful for testing algorithms before running large workloads
- Allows reproducible test cases with fixed seed

**Key concepts**:
- Random number generation
- File I/O
- Test data creation
- Reference string patterns

**Usage**:
```bash
gcc -o 1 1.c
./1
# Creates testInput.txt
```

### `2.c` - Page Replacement Framework/Template
**Purpose**: Provides skeleton code shared by all replacement algorithms.

**What it does**:
- Reads page numbers from stdin
- Maintains in-memory page cache
- Reports page faults to stdout
- Template for implementing specific algorithms
- Shows common structure

**Key concepts**:
- Cache data structure
- Page fault detection
- Input stream processing
- Modular algorithm design

**Usage**:
```bash
gcc -o 2 2.c
cat testInput.txt | ./2 <cacheSize>
```

### `fifo.c` - FIFO Page Replacement
**Purpose**: Implement First-In-First-Out page replacement.

**What it does**:
- Reads page reference string from stdin
- Maintains cache of specified size
- Evicts oldest page when cache full
- Outputs page numbers that cause faults (one per line)
- Silent on page hits

**Algorithm**:
```
1. Check if page in cache (linear search)
2. If HIT: Continue (no output)
3. If MISS:
   a. Print page number (page fault)
   b. If cache not full: Add to cache
   c. If cache full: Replace page at FIFO pointer
   d. Advance FIFO pointer (circular)
```

**Data structures**:
```c
int cache[cacheSize];     // Circular buffer
int next_to_replace = 0;  // FIFO pointer
```

**Usage**:
```bash
gcc -o fifo fifo.c
cat accesses.txt | ./fifo 10     # 10-frame cache
cat accesses.txt | ./fifo 10 | wc -l   # Count page faults
```

**Sample output**:
```
1
2
3
4
5
...
# Each number is a page fault
```

### `lru.c` - LRU Page Replacement
**Purpose**: Implement Least Recently Used page replacement.

**What it does**:
- Tracks last access time for each cached page
- On hit: Updates timestamp/counter
- On miss: Evicts page with oldest timestamp
- More complex but better performance than FIFO

**Algorithm**:
```
1. Check if page in cache
2. If HIT:
   a. Update last_used timestamp
   b. Continue (no output)
3. If MISS:
   a. Print page number (page fault)
   b. If cache not full: Add to cache
   c. If cache full:
      - Find page with minimum last_used
      - Replace that page
   d. Set last_used = current_time
```

**Data structures**:
```c
typedef struct {
    int pageno;
    int last_used;    // Timestamp
} ref_page;

ref_page cache[cacheSize];
int time = 0;  // Global time counter
```

**Usage**:
```bash
gcc -o lru lru.c
cat accesses.txt | ./lru 10      # 10-frame cache
```

**Performance**: Typically 10-30% fewer faults than FIFO on real workloads.

### `sec_chance.c` - Second Chance Algorithm
**Purpose**: Implement Clock (Second Chance) page replacement.

**What it does**:
- FIFO with reference bit giving pages a "second chance"
- Clock hand scans circularly
- Pages with reference bit = 1 get cleared and skipped
- Pages with reference bit = 0 get evicted
- Approximates LRU behavior

**Algorithm**:
```
1. Check if page in cache
2. If HIT:
   a. Set reference_bit = 1
   b. Continue
3. If MISS:
   a. Print page number (page fault)
   b. While true:
      - If cache[clock_hand].reference_bit == 0:
        * Replace this page
        * Break
      - Else:
        * Set reference_bit = 0
        * Advance clock_hand
   c. Set new page's reference_bit = 1
```

**Data structures**:
```c
typedef struct {
    int pageno;
    int reference_bit;
} ref_page;

ref_page cache[cacheSize];
int clock_hand = 0;
```

**Usage**:
```bash
gcc -o sec_chance sec_chance.c
cat accesses.txt | ./sec_chance 10
```

**Performance**: Between FIFO and LRU, closer to LRU.

### `accesses.txt`
**Purpose**: Large-scale reference string for realistic testing.

**Contents**:
- 10,000 page accesses
- Realistic access patterns
- Used for performance comparisons
- Sufficient to show algorithmic differences

**Format**:
```
1
2
3
1
4
...
```
One page number per line.

### `testInput.txt`
**Purpose**: Smaller test file for quick validation.

**Contents**:
- Generated by `1.c`
- Smaller reference string (~100-1000 accesses)
- Quick manual verification
- Custom patterns for testing edge cases

### `Makefile`
**Purpose**: Build automation for all algorithms.

**What it does**:
- Compiles all `.c` files to executables
- Provides clean target
- Consistent compiler flags
- Batch builds

**Usage**:
```bash
make all      # Build everything
make fifo     # Build FIFO only
make lru      # Build LRU only
make clean    # Remove binaries
```

**Typical contents**:
```makefile
CC = gcc
CFLAGS = -Wall -O2

all: 1 2 fifo lru sec_chance

clean:
    rm -f 1 2 fifo lru sec_chance
```

### `lab7.sh`
**Purpose**: Automated comprehensive testing script.

**What it does**:
- Builds all programs
- Runs all algorithms on both test files
- Sweeps through multiple cache sizes
- Collects fault counts for comparison
- Generates performance data

**Usage**:
```bash
chmod +x lab7.sh
./lab7.sh
```

**Sample operations**:
```bash
# Test each algorithm with cache sizes 5, 10, 20, 50
for size in 5 10 20 50; do
    echo "Cache size: $size"
    echo -n "FIFO: "
    cat accesses.txt | ./fifo $size | wc -l
    echo -n "LRU: "
    cat accesses.txt | ./lru $size | wc -l
    echo -n "Second Chance: "
    cat accesses.txt | ./sec_chance $size | wc -l
done
```

### `visual.py`
**Purpose**: Visualization of page fault data (optional).

**What it does**:
- Plots page fault counts vs. cache size
- Compares algorithms visually
- Generates graphs for lab report
- Explores data patterns

**Key libraries**:
- `matplotlib` for plotting
- `pandas` for data manipulation
- `numpy` for calculations

**Usage**:
```bash
python3 visual.py
# Generates plot.png or shows interactive plot
```

**Sample visualization**:
- X-axis: Cache size (frames)
- Y-axis: Page faults
- Lines: One per algorithm (FIFO, LRU, Second Chance)

### `skeleton.c`
**Purpose**: Template/starter code for implementing algorithms.

**What it does**:
- Provides basic structure
- Shows input/output handling
- Skeleton for cache management
- Starting point for implementations

## How to Build

### Using Makefile (Recommended)
```bash
make          # Build all programs
make clean    # Remove executables
```

### Manual Compilation
```bash
gcc -o 1 1.c
gcc -o 2 2.c
gcc -o fifo fifo.c
gcc -o lru lru.c
gcc -o sec_chance sec_chance.c
```

### With Optimization
```bash
gcc -O2 -Wall -o fifo fifo.c
gcc -O2 -Wall -o lru lru.c
gcc -O2 -Wall -o sec_chance sec_chance.c
```

## How to Run

### Generate Test Data
```bash
./1               # Creates testInput.txt
```

### Run Individual Algorithm
```bash
cat testInput.txt | ./fifo 5         # FIFO with 5 frames
cat testInput.txt | ./lru 10         # LRU with 10 frames
cat accesses.txt | ./sec_chance 20   # Second Chance with 20 frames
```

### Count Page Faults
```bash
cat accesses.txt | ./fifo 10 | wc -l     # Count FIFO faults
cat accesses.txt | ./lru 10 | wc -l      # Count LRU faults
cat accesses.txt | ./sec_chance 10 | wc -l  # Count Second Chance faults
```

### Compare All Algorithms
```bash
echo "Cache size: 10 frames"
echo -n "FIFO faults: "
cat accesses.txt | ./fifo 10 | wc -l
echo -n "LRU faults: "
cat accesses.txt | ./lru 10 | wc -l
echo -n "Second Chance faults: "
cat accesses.txt | ./sec_chance 10 | wc -l
```

### Comprehensive Test (Multiple Cache Sizes)
```bash
./lab7.sh         # Runs full test matrix
```

### Visualize Results
```bash
python3 visual.py   # Generate plots
```

## Expected Behavior and Output

### Normal Operation
- Programs read page numbers from stdin
- Each page fault prints the page number
- Page hits produce no output
- Exit cleanly after processing all input

### Output Format
Each algorithm outputs only faulting pages:
```
1
2
3
5
...
```
No additional text, just page numbers.

### Performance Expectations
For typical workloads with 10,000 accesses:

| Cache Size | FIFO | Second Chance | LRU |
|------------|------|---------------|-----|
| 5 frames   | 9500 | 9200          | 8800 |
| 10 frames  | 9000 | 8500          | 7500 |
| 20 frames  | 8000 | 7200          | 6000 |
| 50 frames  | 6000 | 5500          | 4500 |

**Pattern**: LRU < Second Chance < FIFO (fewer faults is better)

### Belady's Anomaly Example
May observe with FIFO on certain reference strings:
```
4 frames: 10 faults
5 frames: 12 faults  ← Counterintuitive!
```

## Key Concepts and Learning Outcomes

### Cache Hit Check
```c
bool hit = false;
for (int i = 0; i < cacheSize; i++) {
    if (cache[i].pageno == page_num) {
        hit = true;
        // Update metadata (for LRU, Second Chance)
        break;
    }
}
```

### FIFO Replacement
```c
// Circular buffer approach
cache[next_to_replace].pageno = page_num;
next_to_replace = (next_to_replace + 1) % cacheSize;
```

### LRU Replacement
```c
// Find least recently used page
int lru_index = 0;
int min_time = cache[0].last_used;

for (int i = 1; i < cacheSize; i++) {
    if (cache[i].last_used < min_time) {
        min_time = cache[i].last_used;
        lru_index = i;
    }
}

// Replace LRU page
cache[lru_index].pageno = page_num;
cache[lru_index].last_used = current_time;
```

### Second Chance Replacement
```c
while (true) {
    if (cache[clock_hand].reference_bit == 0) {
        // Evict this page
        cache[clock_hand].pageno = page_num;
        cache[clock_hand].reference_bit = 1;
        clock_hand = (clock_hand + 1) % cacheSize;
        break;
    } else {
        // Give second chance
        cache[clock_hand].reference_bit = 0;
        clock_hand = (clock_hand + 1) % cacheSize;
    }
}
```

## What I Learned
- I learned how to stream reference strings through stdin and reuse the same driver for multiple algorithms.
- I learned the implementation trade-offs between FIFO, LRU, and Second Chance and how fault counts shift with cache size.
- I learned to script comprehensive test matrices (`lab7.sh`) so I can gather reproducible evidence for my lab report with a single command.
- I learned about Belady's Anomaly and why some algorithms behave counterintuitively.
- I learned why LRU is theoretically superior but more expensive to implement in hardware.
- I learned how Second Chance (Clock) provides a practical middle ground.
- I learned to design experiments that isolate algorithmic differences through controlled comparisons.
- I learned how locality of reference dramatically affects page replacement performance.

## Common Issues and Troubleshooting

### No Output from Program
**Problem**: Running algorithm produces no output
**Solution**: This means no page faults occurred (cache larger than working set)
```bash
# Check if input is being read
cat testInput.txt | ./fifo 1000  # Very large cache = few/no faults
cat testInput.txt | ./fifo 1     # Tiny cache = many faults
```

### Incorrect Fault Counts
**Problem**: Fault count doesn't match expected
**Solution**:
- Verify cache initialization (all entries marked as empty/-1)
- Check hit detection logic
- Ensure proper circular index arithmetic `(i+1) % size`
- Validate input parsing (check for blank lines)

### Program Hangs on Input
**Problem**: Program waits indefinitely
**Solution**: Ensure input is provided via pipe or redirection
```bash
# Wrong: no input provided
./fifo 10

# Right: input from file
cat accesses.txt | ./fifo 10

# Or
./fifo 10 < accesses.txt
```

### Segmentation Fault
**Problem**: Program crashes
**Causes**:
- Array index out of bounds
- Cache size <= 0
- Uninitialized variables

**Solution**:
```c
// Add bounds checking
if (cacheSize <= 0) {
    fprintf(stderr, "Cache size must be positive\n");
    return 1;
}

// Initialize all cache entries
for (int i = 0; i < cacheSize; i++) {
    cache[i].pageno = -1;  // Empty marker
}
```

### FIFO Showing Belady's Anomaly
**Problem**: More frames = more faults (confusing!)
**Solution**: This is expected behavior for FIFO with certain reference strings. Document as observation, not bug.

## Performance Analysis

### Measuring Page Faults
```bash
# Create performance comparison
for alg in fifo lru sec_chance; do
    echo "$alg algorithm:"
    for size in 5 10 20 50 100; do
        faults=$(cat accesses.txt | ./$alg $size | wc -l)
        echo "  Size $size: $faults faults"
    done
done
```

### Hit Rate Calculation
```
Total accesses: wc -l < accesses.txt
Page faults: cat accesses.txt | ./fifo 10 | wc -l
Page hits: Total - Faults
Hit rate: Hits / Total × 100%
```

### Cache Size Impact
Typical pattern:
- Small cache: ~90-95% fault rate
- Medium cache: ~50-70% fault rate
- Large cache: ~10-30% fault rate
- Very large cache: ~0-5% fault rate (entire working set fits)

## Further Exploration
- Implement Optimal (Belady's) algorithm (requires future knowledge)
- Add support for different page sizes
- Simulate TLB (Translation Lookaside Buffer) with page cache
- Implement working set algorithm
- Add page reference statistics (frequency distribution)
- Create custom reference strings to trigger Belady's Anomaly
- Implement LRU with different data structures (heap, doubly-linked list)
- Add dirty bit tracking for write-back policy simulation
- Measure algorithmic complexity (time per page access)
- Implement adaptive algorithms that switch based on workload
- Compare with random replacement as baseline
- Simulate multi-level page tables
- Add memory pressure simulation (variable cache sizes over time)
- Implement clock-pro or other advanced algorithms
