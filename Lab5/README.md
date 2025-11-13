# Lab 5 – Synchronization Primitives

## Overview
This lab introduces thread synchronization mechanisms essential for safe concurrent programming. Building on Lab 4's basic threads, you'll learn how to protect critical sections, coordinate complex workflows, and solve classic concurrency problems like producer-consumer. Understanding mutexes and semaphores is crucial for writing correct multithreaded programs that avoid race conditions, deadlocks, and data corruption.

## Purpose
Apply pthread synchronization tools—mutexes, unnamed and named semaphores—to protect critical sections and coordinate producer/consumer style workflows.

## Prerequisites
- C compiler with pthread support
- Completed Lab 4 (basic threads)
- Understanding of race conditions and critical sections
- Familiarity with shared memory in multithreading
- Knowledge of concurrent programming concepts

## Core Concepts

### Race Conditions
**Problem**: Multiple threads accessing shared data without synchronization leads to unpredictable, incorrect results.

Example:
```c
// Shared counter
int counter = 0;

// Two threads both execute:
counter++;  // Not atomic! Actually: load, increment, store

// Result: Lost updates! Final counter may be 1 instead of 2
```

### Critical Section
A **critical section** is code that accesses shared resources and must not be executed by multiple threads simultaneously.

**Solution**: Use synchronization primitives to ensure mutual exclusion (mutex).

### Synchronization Primitives

#### 1. Mutex (Mutual Exclusion Lock)
- **Purpose**: Binary lock for protecting critical sections
- **States**: Locked or unlocked
- **Ownership**: Thread that locks must unlock
- **Use case**: Protecting shared data structures

#### 2. Semaphore
- **Purpose**: Counter-based synchronization primitive
- **States**: Integer value (0, 1, 2, ...)
- **Ownership**: No ownership, any thread can signal
- **Types**:
  - Binary semaphore (0 or 1) - similar to mutex
  - Counting semaphore (0 to N) - resource counting
- **Use case**: Signaling between threads, resource pools

#### 3. Named vs. Unnamed Semaphores
| Feature | Named Semaphores | Unnamed Semaphores |
|---------|------------------|-------------------|
| **Scope** | System-wide (across processes) | Process-local |
| **Creation** | `sem_open()` | `sem_init()` |
| **Cleanup** | `sem_close()`, `sem_unlink()` | `sem_destroy()` |
| **Identifier** | String name (e.g., "/mysem") | Memory address |
| **IPC** | Yes (between processes) | No (threads only) |

### Producer-Consumer Problem
**Classic synchronization challenge**:
- Producer threads generate data and add to buffer
- Consumer threads remove data from buffer
- Buffer has limited capacity (bounded buffer)
- Must prevent: buffer overflow, buffer underflow, race conditions

**Solution requires**:
1. **Mutex**: Protect buffer structure itself
2. **Empty slots semaphore**: Track available space
3. **Full slots semaphore**: Track available items

## Files in This Lab

### `threadSync.c` - Semaphore for Critical Section
**Purpose**: Demonstrate named semaphore protecting a critical section.

**What it does**:
- Creates 10 threads attempting to enter critical section
- Uses named semaphore initialized to 1 (binary semaphore)
- Only one thread executes critical section at a time
- Each thread sleeps for 1 second in critical section
- Threads now execute serially instead of racing

**Key concepts**:
- Named semaphore creation with `sem_open()`
- `sem_wait()` to acquire (decrement) semaphore
- `sem_post()` to release (increment) semaphore
- Signal handler for cleanup (`SIGINT`)
- Demonstrates mutual exclusion

**Usage**:
```bash
gcc -pthread -o threadSync threadSync.c
./threadSync
```

**Expected output**:
```
Thread 0 is now attempting ....
Thread 1 is now attempting ....
Thread 0 is running in its Critical Section........
Thread 1 is running in its Critical Section........
...
```

**Comparison with Lab 4**: Without synchronization, threads would interleave randomly. With semaphore, they execute critical section sequentially.

### `2.c` - Mutex for Critical Section
**Purpose**: Same critical section protection using pthread mutex instead of semaphore.

**What it does**:
- Implements identical functionality to `threadSync.c`
- Uses `pthread_mutex_t` instead of semaphore
- Demonstrates mutex API: `pthread_mutex_init()`, `pthread_mutex_lock()`, `pthread_mutex_unlock()`, `pthread_mutex_destroy()`
- Compares mutex vs. semaphore API ergonomics

**Key concepts**:
- Mutex initialization and destruction
- `pthread_mutex_lock()` - acquire mutex (blocking)
- `pthread_mutex_unlock()` - release mutex
- Mutex ownership semantics
- Cleaner API for simple mutual exclusion

**Usage**:
```bash
gcc -pthread -o 2 2.c
./2
```

**Mutex vs. Semaphore**:
- **Mutex**: Simpler API for binary locking
- **Semaphore**: More flexible, supports counting
- **Preference**: Use mutex for simple critical sections, semaphore for counting/signaling

### `3.c` - Producer-Consumer with Bounded Buffer
**Purpose**: Solve the classic bounded-buffer producer-consumer problem.

**What it does**:
- Implements circular buffer with 9 slots
- Producer thread generates 20 items
- Consumer thread removes 20 items
- Uses three semaphores: `mutex`, `empty`, `full`
- Demonstrates proper ordering to prevent deadlock

**Algorithm**:
```
Producer:
1. wait(empty)     // Wait for empty slot
2. wait(mutex)     // Lock buffer
3. Add item to buffer[in]
4. in = (in + 1) % BUFFER_SIZE
5. post(mutex)     // Unlock buffer
6. post(full)      // Signal item available

Consumer:
1. wait(full)      // Wait for item
2. wait(mutex)     // Lock buffer
3. Remove item from buffer[out]
4. out = (out + 1) % BUFFER_SIZE
5. post(mutex)     // Unlock buffer
6. post(empty)     // Signal slot available
```

**Key concepts**:
- Bounded buffer with circular indices
- Three-semaphore solution
- Preventing buffer overflow/underflow
- Critical ordering to avoid deadlock
- Producer-consumer coordination

**Usage**:
```bash
gcc -pthread -o 3 3.c
./3
```

**Expected output**:
```
Producer: placed item 1 at 0
Producer: placed item 2 at 1
Consumer: removed item 1 from 0
Producer: placed item 3 at 2
Consumer: removed item 2 from 1
...
```

### `4.c` - Advanced Synchronization Patterns
**Purpose**: More complex synchronization scenarios.

**What it does**:
- Extends producer-consumer concepts
- May include multiple producers/consumers
- Demonstrates additional synchronization patterns
- Shows real-world concurrency scenarios

**Key concepts**:
- Multiple producers and consumers
- Fair scheduling
- Starvation prevention
- Complex coordination

### `Makefile`
**Purpose**: Build automation for all lab programs.

**What it does**:
- Compiles all programs with `-pthread` flag
- Provides targets for individual builds
- Includes clean target
- Ensures consistent compilation

**Usage**:
```bash
make              # Build all
make threadSync   # Build specific program
make clean        # Remove binaries
```

## How to Build

### Using Makefile
```bash
make          # Build all programs
make clean    # Remove executables
```

### Manual Compilation
```bash
gcc -pthread -o threadSync threadSync.c
gcc -pthread -o 2 2.c
gcc -pthread -o 3 3.c
gcc -pthread -o 4 4.c
```

**Important**: Always use `-pthread` flag!

### Compilation Flags
```bash
gcc -pthread -Wall -O2 -o 3 3.c
# -pthread: Link pthread library
# -Wall: Enable all warnings
# -O2: Optimize performance
```

## How to Run

### Basic Critical Section Protection
```bash
./threadSync      # Using named semaphore
./2               # Using mutex
```

### Producer-Consumer
```bash
./3               # Single producer, single consumer
./4               # Multiple producers/consumers
```

### Observing Race Conditions
To see the difference synchronization makes:
1. Comment out `sem_wait()` and `sem_post()` calls
2. Recompile and run
3. Observe garbled output and incorrect behavior

### Monitoring Semaphores
```bash
# Linux: List named semaphores
ls -l /dev/shm/sem.*

# macOS: Named semaphores in kernel
ipcs -s
```

## Expected Behavior and Output

### Critical Section Programs (threadSync.c, 2.c)
- Threads announce attempt to enter critical section
- Only one thread executes critical section at a time
- 1-second delay makes serialization visible
- No interleaved output during critical section

### Producer-Consumer (3.c)
- Producer places items in buffer sequentially
- Consumer removes items in same order (FIFO)
- Buffer never overflows (producer blocks when full)
- Buffer never underflows (consumer blocks when empty)
- Clean producer/consumer interleaving

### Without Synchronization
- Garbled output
- Data corruption
- Race conditions
- Unpredictable behavior
- Possible crashes

## Key Concepts and Learning Outcomes

### Using Named Semaphores
```c
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

// Create/open semaphore
sem_t *sem = sem_open("/mysem", O_CREAT | O_EXCL,
                      S_IRUSR | S_IWUSR, 1);

// Use semaphore
sem_wait(sem);   // P operation, decrement, block if 0
// ... critical section ...
sem_post(sem);   // V operation, increment

// Cleanup
sem_close(sem);
sem_unlink("/mysem");
```

### Using Mutexes
```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Or dynamic initialization
pthread_mutex_init(&mutex, NULL);

// Use mutex
pthread_mutex_lock(&mutex);
// ... critical section ...
pthread_mutex_unlock(&mutex);

// Cleanup
pthread_mutex_destroy(&mutex);
```

### Producer-Consumer Pattern
```c
// Initialization
sem_t *mutex = sem_open("/mutex", O_CREAT, 0644, 1);
sem_t *empty = sem_open("/empty", O_CREAT, 0644, BUFFER_SIZE);
sem_t *full  = sem_open("/full",  O_CREAT, 0644, 0);

// Producer
void *producer(void *arg) {
    while (have_items) {
        item = produce_item();
        sem_wait(empty);        // Wait for space
        sem_wait(mutex);        // Lock buffer
        buffer[in] = item;
        in = (in + 1) % SIZE;
        sem_post(mutex);        // Unlock buffer
        sem_post(full);         // Signal item ready
    }
}

// Consumer
void *consumer(void *arg) {
    while (need_items) {
        sem_wait(full);         // Wait for item
        sem_wait(mutex);        // Lock buffer
        item = buffer[out];
        out = (out + 1) % SIZE;
        sem_post(mutex);        // Unlock buffer
        sem_post(empty);        // Signal space available
        consume_item(item);
    }
}
```

### Critical Ordering Rules
1. **Producer**: Always `wait(empty)` before `wait(mutex)`
2. **Consumer**: Always `wait(full)` before `wait(mutex)`
3. **Both**: Always `post(mutex)` before `post(empty/full)`
4. **Violation**: Wrong order causes deadlock!

## What I Learned
- I learned the practical differences between mutexes and semaphores for guarding shared state.
- I learned to structure producer/consumer loops with separate "slots available" vs. "items available" semaphores.
- I learned to wrap semaphore lifecycle management (`sem_open`, `sem_close`, `sem_unlink`) in cleanup logic so repeated runs stay reliable.
- I learned how critical proper ordering (`wait` before touching the buffer, `post` afterward) is for correctness.
- I learned that race conditions are real and synchronization is not optional in concurrent code.
- I learned how to use signal handlers for clean semaphore cleanup on interrupt.
- I learned the difference between named and unnamed semaphores and when to use each.
- I learned that synchronization has performance costs but correctness is paramount.

## Common Issues and Troubleshooting

### Deadlock
**Problem**: Program hangs, threads waiting forever
**Causes**:
- Wrong ordering of semaphore operations
- Acquiring locks in different orders
- Forgetting to release locks

**Solution**:
```c
// Bad: Can deadlock if producer/consumer both do this
sem_wait(mutex);
sem_wait(empty);  // Might block while holding mutex!

// Good: Acquire resource semaphore first
sem_wait(empty);
sem_wait(mutex);
```

### Semaphore Already Exists
**Problem**: `sem_open: File exists`
**Solution**: Clean up from previous run
```bash
# Remove named semaphore
rm /dev/shm/sem.lab5_mutex*  # Linux
# macOS: semaphores auto-cleanup

# Or in code: Always unlink before creating
sem_unlink("/mysem");
sem = sem_open("/mysem", O_CREAT | O_EXCL, 0644, 1);
```

### Resource Leak
**Problem**: Semaphores not cleaned up after crash
**Solution**: Add signal handler
```c
void cleanup(int sig) {
    sem_close(mutex);
    sem_unlink("/mutex");
    exit(0);
}

signal(SIGINT, cleanup);
```

### Mutex Not Initialized
**Problem**: `Segmentation fault` or undefined behavior
**Solution**: Always initialize before use
```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Static
// or
pthread_mutex_init(&mutex, NULL);  // Dynamic
```

### Race Condition Persists
**Problem**: Still seeing garbled output
**Solution**:
- Ensure ALL accesses to shared data are protected
- Check that lock is acquired before critical section
- Verify lock is released after critical section
- Use helgrind: `valgrind --tool=helgrind ./program`

### Buffer Overflow/Underflow
**Problem**: Array index out of bounds
**Solution**: Verify circular buffer logic
```c
// Correct circular index
in = (in + 1) % BUFFER_SIZE;   // Not just in++
out = (out + 1) % BUFFER_SIZE; // Not just out++
```

## Debugging and Testing

### Detect Race Conditions
```bash
# Helgrind: Thread error detector
valgrind --tool=helgrind ./3

# DRD: Another race detector
valgrind --tool=drd ./3
```

### Debug Deadlock
```bash
# Attach GDB to hung process
ps aux | grep ./3
gdb -p <pid>
(gdb) info threads
(gdb) thread apply all bt   # Backtrace all threads
```

### Stress Testing
```bash
# Run repeatedly to expose rare race conditions
for i in {1..100}; do
    ./3
    echo "Run $i completed"
done
```

## Performance Considerations

### Synchronization Overhead
- Lock acquisition/release costs ~50-100 CPU cycles
- Contention (waiting for lock) can be expensive
- Minimize critical section size
- Hold locks for shortest time possible

### Scalability
```
Speedup limited by:
- Amdahl's Law (serial portions)
- Lock contention (threads waiting)
- False sharing (cache line bouncing)
```

### Best Practices
1. **Minimize critical sections**: Only protect what must be protected
2. **Avoid nested locks**: Simplifies deadlock prevention
3. **Use mutex for simple cases**: Simpler than semaphores
4. **Use counting semaphores for resources**: Natural fit for pools
5. **Always clean up**: Prevent resource leaks

## Further Exploration
- Implement multiple producers and multiple consumers (4.c)
- Add bounded sleep/delays to producer/consumer to vary speeds
- Measure performance impact of different buffer sizes
- Implement reader-writer locks with reader/writer preference
- Solve the dining philosophers problem
- Implement a thread-safe queue data structure
- Add condition variables (`pthread_cond_t`) for more efficient waiting
- Compare semaphore vs. mutex vs. condition variable performance
- Implement lock-free data structures using atomic operations
- Explore priority inversion and priority inheritance
- Test with ThreadSanitizer: `gcc -fsanitize=thread`
- Implement barrier synchronization for thread coordination
