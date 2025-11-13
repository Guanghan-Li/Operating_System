# Lab 4 – Intro to Pthreads

## Overview
This lab introduces POSIX threads (pthreads), transitioning from multi-process programming to multi-threaded programming. Threads share the same address space, making data sharing simpler than with separate processes, but introducing new challenges around synchronization and race conditions. Through matrix multiplication examples, you'll explore parallelization strategies and performance trade-offs between different granularities of work distribution.

## Purpose
Move from multi-process coordination into multi-threaded programming so I can share memory, divide work with threads, and reason about synchronization primitives.

## Prerequisites
- C compiler with pthread support (gcc or clang)
- Pthread library (typically included in Unix/Linux/macOS)
- Completed Labs 1-3 (process creation and IPC)
- Understanding of matrix multiplication
- Basic knowledge of parallel programming concepts
- Familiarity with compilation flags

## Key Concepts

### Threads vs. Processes
| Aspect | Processes | Threads |
|--------|-----------|---------|
| **Address Space** | Separate | Shared |
| **Creation Cost** | High (fork overhead) | Low (lightweight) |
| **Communication** | IPC needed (pipes, shared memory) | Direct memory access |
| **Context Switch** | Expensive | Cheaper |
| **Isolation** | Strong (protected) | Weak (shared memory) |
| **Use Case** | Independent tasks | Collaborative parallel work |

### POSIX Threads (pthreads)
- **Thread**: Lightweight execution unit within a process
- **Shared resources**: Global variables, heap, file descriptors
- **Private resources**: Stack, registers, thread-local storage
- **Concurrency**: Multiple threads execute concurrently (or in parallel on multi-core)
- **Race conditions**: Unsynchronized access to shared data causes bugs

### Parallelization Strategies
1. **Coarse-grained**: Few threads, large work units (e.g., one thread per matrix row)
   - Lower overhead, better cache locality
   - May have load imbalance
2. **Fine-grained**: Many threads, small work units (e.g., one thread per matrix cell)
   - Higher overhead, more context switches
   - Better load distribution but worse performance due to overhead

## Files in This Lab

### `1.c` - Basic Thread Creation
**Purpose**: Introductory program demonstrating thread creation and joining.

**What it does**:
- Creates 20 worker threads
- Each thread prints its identifier
- Main thread waits for all workers to complete using `pthread_join()`
- Demonstrates basic pthread lifecycle

**Key concepts**:
- `pthread_create()` - spawn new thread
- `pthread_join()` - wait for thread termination
- Thread function signature: `void* function(void* arg)`
- Passing arguments to threads
- Thread ID management

**Usage**:
```bash
gcc -pthread -o 1 1.c
./1
```

**Expected output** (order may vary):
```
Thread 0 is running
Thread 1 is running
Thread 5 is running
Thread 2 is running
...
```

### `4.c` - Matrix Multiplication (Per-Row Parallelism)
**Purpose**: Coarse-grained parallelization with one thread per result matrix row.

**What it does**:
- Takes three arguments: N, M, L (matrix dimensions)
- Creates two matrices: A (N×M) and B (M×L)
- Spawns N threads, one for each row of result matrix C (N×L)
- Each thread computes all elements in its assigned row
- Prints all three matrices after computation

**Matrix multiplication**:
```
C[i][j] = Σ(k=0 to M-1) A[i][k] × B[k][j]
```

**Key concepts**:
- Coarse-grained parallelism (row-level)
- Shared memory between threads (matrices are global)
- No synchronization needed (threads write to different rows)
- Natural data parallelism
- Work distribution strategy

**Usage**:
```bash
gcc -pthread -o 4 4.c
./4 3 3 3        # Multiply 3×3 by 3×3 matrices
./4 10 5 8       # Multiply 10×5 by 5×8 matrices
```

**Performance characteristics**:
- Thread count: N (number of rows in result)
- Best for: Moderate N (10-1000 rows)
- Overhead: Low per thread
- Cache behavior: Good locality within row computation

### `5.c` - Matrix Multiplication (Per-Cell Parallelism)
**Purpose**: Fine-grained parallelization with one thread per result matrix cell.

**What it does**:
- Same matrix dimensions as `4.c`: N, M, L
- Spawns N×L threads, one for each cell of result matrix
- Each thread computes a single element C[i][j]
- Demonstrates fine-grained parallelism and its overhead

**Key concepts**:
- Fine-grained parallelism (cell-level)
- Thread creation overhead at scale
- Diminishing returns from over-parallelization
- Comparison point for granularity analysis
- Amdahl's Law implications

**Usage**:
```bash
gcc -pthread -o 5 5.c
./5 3 3 3        # Creates 9 threads (3×3 result)
./5 10 10 10     # Creates 100 threads (10×10 result)
./5 100 100 100  # Creates 10,000 threads (!!)
```

**Performance characteristics**:
- Thread count: N×L (can be huge!)
- Best for: Small matrices only
- Overhead: Very high for large matrices
- Cache behavior: Poor (each thread does minimal work)
- **Note**: Often slower than per-row or even sequential!

### `Makefile`
**Purpose**: Automated build system for all lab programs.

**What it does**:
- Defines compilation rules with `-pthread` flag
- Builds all numbered programs
- Provides clean target for removing binaries
- Ensures consistent compiler flags

**Usage**:
```bash
make           # Build all programs
make 1         # Build only program 1
make 4         # Build only program 4
make clean     # Remove all executables
```

**Typical contents**:
```makefile
CC = gcc
CFLAGS = -pthread -Wall

all: 1 4 5

1: 1.c
    $(CC) $(CFLAGS) -o 1 1.c

4: 4.c
    $(CC) $(CFLAGS) -o 4 4.c

5: 5.c
    $(CC) $(CFLAGS) -o 5 5.c

clean:
    rm -f 1 4 5
```

### `lab4.sh`
**Purpose**: Automated test script for running experiments.

**What it does**:
- Builds all programs using Makefile
- Runs programs with various parameters
- Times execution for performance comparison
- Facilitates systematic testing

**Usage**:
```bash
chmod +x lab4.sh
./lab4.sh
```

### `observation.txt`
**Purpose**: Document experimental results and analysis.

**Contains**:
- Runtime measurements for different matrix sizes
- Comparison of per-row vs. per-cell performance
- Analysis of overhead and scaling behavior
- Observations about thread scheduling
- Answers to lab questions
- Performance bottleneck identification

## How to Build

### Using Makefile (Recommended)
```bash
make          # Build all programs
make clean    # Remove executables
```

### Manual Compilation
```bash
gcc -pthread -o 1 1.c
gcc -pthread -o 4 4.c
gcc -pthread -o 5 5.c
```

**Important**: Always use `-pthread` flag for thread support!

### Common Compilation Flags
```bash
gcc -pthread -Wall -O2 -o 4 4.c
# -pthread: Enable pthread support
# -Wall: Show all warnings
# -O2: Optimize for performance
```

## How to Run

### Basic Thread Demo
```bash
./1                    # Run 20 threads printing IDs
```

### Matrix Multiplication - Per Row
```bash
./4 5 5 5             # Small 5×5 matrices
./4 100 100 100       # Larger 100×100 matrices
./4 1000 1000 1000    # Stress test with 1000×1000
```

### Matrix Multiplication - Per Cell
```bash
./5 3 3 3             # Small 3×3 (9 threads)
./5 10 10 10          # Medium 10×10 (100 threads)
./5 50 50 50          # Large 50×50 (2500 threads!)
```

### Performance Comparison
```bash
# Compare runtimes
time ./4 100 100 100
time ./5 100 100 100

# Per-row should be much faster for reasonable sizes
```

### Monitoring Threads
```bash
# In one terminal
./4 1000 1000 1000 &
PID=$!

# In another terminal
top -H -p $PID        # Linux: show threads
ps -M $PID            # macOS: show threads
```

## Expected Behavior and Output

### Program 1 Output
```
Thread 0 is running
Thread 3 is running
Thread 1 is running
Thread 2 is running
...
```
**Note**: Order is non-deterministic due to CPU scheduling

### Program 4/5 Output
```
Matrix A:
    7     3     5
    8     1     2
    9     4     6

Matrix B:
    1     0     3
    5     2     7
    4     8     9

Matrix C (Result):
   42    46    87
   21    18    49
   53    56    97
```

### Performance Patterns
- **Small matrices**: Both approaches perform similarly
- **Medium matrices** (50×50 to 500×500): Per-row is faster
- **Large matrices** (1000×1000+): Per-row significantly faster
- **Huge matrices**: May need even coarser granularity

## Key Concepts and Learning Outcomes

### Creating Threads
```c
pthread_t thread_id;
void *thread_function(void *arg) {
    int id = (int)(size_t)arg;
    printf("Thread %d running\n", id);
    return NULL;
}

// Create thread
pthread_create(&thread_id, NULL, thread_function, (void *)(size_t)id);
```

### Waiting for Threads
```c
pthread_t threads[10];

// Create threads
for (int i = 0; i < 10; i++) {
    pthread_create(&threads[i], NULL, worker, (void *)(size_t)i);
}

// Wait for all to finish
for (int i = 0; i < 10; i++) {
    pthread_join(threads[i], NULL);
}
```

### Sharing Data Between Threads
```c
// Global variables are shared by all threads
int shared_counter = 0;
double **shared_matrix;

void *thread_function(void *arg) {
    // All threads can read/write shared_counter and shared_matrix
    shared_counter++;  // WARNING: Race condition without synchronization!
    return NULL;
}
```

### Thread Function Signature
```c
void *my_thread_function(void *argument) {
    // Cast argument to appropriate type
    int my_id = (int)(size_t)argument;

    // Do work...

    // Return value (can be retrieved by pthread_join)
    return NULL;
}
```

## What I Learned
- I learned how to create, join, and pass arguments to POSIX threads.
- I learned the trade-offs between coarse-grained (per-row) and fine-grained (per-cell) parallelism in a compute-bound task.
- I learned to package build/run steps in scripts so I can reproduce lab results quickly.
- I learned that threads share memory, making data sharing easy but requiring careful synchronization.
- I learned that thread creation has overhead, so more threads isn't always better.
- I learned how to measure and compare performance of different parallelization strategies.
- I learned about Amdahl's Law: parallelization gains are limited by serial portions and overhead.
- I learned that cache locality matters - coarse-grained parallelism often has better cache behavior.

## Common Issues and Troubleshooting

### Undefined Reference to pthread_create
**Problem**: `undefined reference to 'pthread_create'`
**Solution**: Add `-pthread` flag
```bash
gcc -pthread -o 4 4.c    # Correct
gcc -o 4 4.c             # Missing -pthread, will fail
```

### Segmentation Fault
**Problem**: Program crashes with segfault
**Solution**: Check for:
- Proper argument validation
- Correct matrix dimension usage
- Memory allocation success
- Array bounds (i < N, not i <= N)

### Inconsistent Output Order
**Problem**: Thread output appears in random order
**Solution**: This is normal! CPU scheduler determines thread execution order.  Use synchronization (next lab) if deterministic order is needed.

### Performance Worse Than Sequential
**Problem**: Threaded version slower than single-threaded
**Solution**:
- Too many threads (overhead dominates)
- Too small matrices (not enough work per thread)
- Bad cache behavior (threads thrashing cache)
- Try coarser granularity or larger problem size

### Matrix Dimensions Wrong
**Problem**: `Segmentation fault` or wrong results
**Solution**: Matrix multiplication requires compatible dimensions:
- A is N×M
- B is M×L (middle dimension must match!)
- C is N×L

### Thread Creation Fails
**Problem**: `pthread_create` returns non-zero
**Solution**:
- System thread limit reached
- Reduce thread count
- Check `ulimit -u` (max user processes)
```bash
ulimit -u 2048    # Increase limit
```

## Performance Analysis

### Speedup Formula
```
Speedup = T_sequential / T_parallel
Efficiency = Speedup / num_threads
```

### Expected Results
For 100×100 matrices on 4-core system:
- Sequential: ~100ms
- Per-row (100 threads): ~30-50ms (2-3× speedup)
- Per-cell (10,000 threads): ~200ms (slower due to overhead!)

### Amdahl's Law
Maximum speedup with P processors:
```
Speedup = 1 / ((1-F) + F/P)
where F = fraction of code that can be parallelized
```

## Further Exploration
- Implement column-wise parallelization and compare with row-wise
- Add timing code to measure speedup and efficiency
- Try different matrix sizes and plot performance curves
- Implement blocked (tiled) matrix multiplication for better cache performance
- Add input validation and error handling
- Compare with OpenMP parallel matrix multiplication
- Experiment with thread affinity (CPU pinning) for better performance
- Profile with `perf` or `valgrind --tool=callgrind`
- Implement Strassen's algorithm with parallel divide-and-conquer
- Try sparse matrix multiplication with different parallelization strategies
- Measure cache miss rates with `perf stat -e cache-misses`
- Compare pthread performance with C++11 threads or OpenMP
