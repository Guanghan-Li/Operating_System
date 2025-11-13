# Lab 2 – Process Creation

## Overview
This lab explores the fundamentals of Unix process management through hands-on C programming exercises. You'll learn how the operating system creates new processes, how parent and child processes coordinate their execution, and how timing affects process interleaving. These concepts are foundational to understanding multitasking, concurrency, and how Unix-like operating systems manage computational resources.

## Purpose
Deepen familiarity with Unix process management by writing tiny C programs that exercise `fork`, `exec`, `wait`, and basic scheduling/delay techniques.

## Prerequisites
- C compiler (gcc or clang)
- Basic understanding of C programming
- Unix/Linux/macOS environment
- Familiarity with command-line compilation
- Understanding of processes and the process lifecycle

## Files in This Lab

### `1.c` - Basic Fork with Delays
**Purpose**: Demonstrate parent-child process creation with controlled interleaving.

**What it does**:
- Creates a single child process using `fork()`
- Parent and child both execute 10 iterations
- Each iteration includes a 3000 microsecond (3ms) delay using `usleep()`
- Parent uses `wait()` to ensure child completes before termination
- Output is indented differently for parent vs. child for visual clarity

**Key concepts**:
- `fork()` return values: negative (error), 0 (child), positive PID (parent)
- Process ID differentiation
- `usleep()` for microsecond-precision delays
- `wait()` for parent-child synchronization
- Race conditions and non-deterministic scheduling

**Sample output**:
```
Before forking.
I am the child process displaying iteration 0
			I am the parent Process displaying iteration 0
I am the child process displaying iteration 1
			I am the parent Process displaying iteration 1
...
```

### `3.c` - Multiple Forks
**Purpose**: Explore process tree creation with multiple `fork()` calls.

**What it does**:
- Demonstrates sequential fork operations
- Creates a simple process hierarchy
- Shows how multiple children relate to a single parent
- Illustrates process tree structure

**Key concepts**:
- Process tree topology
- Multiple child processes
- Parent tracking multiple children
- Process creation cascading

### `5.c` - Fork Without Wait (Zombie Process)
**Purpose**: Demonstrate what happens when parent doesn't wait for child.

**What it does**:
- Creates child process but parent terminates immediately
- Child becomes orphaned or zombie depending on timing
- Illustrates importance of proper process cleanup
- Shows process state transitions

**Key concepts**:
- Zombie processes (defunct processes)
- Orphan processes
- Process reaping
- Init process adoption (`pid 1` inheritance)

### `6.c` - Wait and Process Synchronization
**Purpose**: Proper parent-child synchronization patterns.

**What it does**:
- Demonstrates correct use of `wait()` system call
- Parent blocks until child completes
- Shows deterministic execution ordering
- Prevents zombie processes through proper cleanup

**Key concepts**:
- Blocking system calls
- Process synchronization
- Exit status retrieval
- Deterministic vs. non-deterministic execution

### `7.c` - Sleep vs. Usleep
**Purpose**: Compare different delay mechanisms and their effects.

**What it does**:
- Contrasts `sleep()` (second-level) vs. `usleep()` (microsecond-level)
- Shows granularity differences in timing control
- Demonstrates CPU scheduling quantum effects
- Explores interleaving patterns with different delays

**Key concepts**:
- Timing precision in system calls
- CPU time quantum
- Context switch overhead
- Scheduling algorithms (Round-Robin, etc.)

### `8.c` - Complex Process Coordination
**Purpose**: Advanced multi-process coordination patterns.

**What it does**:
- Combines multiple forks with wait operations
- Creates more complex process hierarchies
- Demonstrates coordinated execution flows
- Shows practical patterns for process management

**Key concepts**:
- Process groups
- Hierarchical waiting
- Complex synchronization
- Real-world process patterns

### `Makefile`
**Purpose**: Automate compilation of all lab programs.

**What it does**:
- Defines build rules for each numbered program
- Enables batch compilation with single `make` command
- Provides `clean` target for removing binaries
- Standardizes compiler flags and options

**Usage**:
```bash
make          # Build all programs
make 1        # Build only 1.c
make clean    # Remove all compiled binaries
```

### `lab2.sh`
**Purpose**: Automated test script for running all programs.

**What it does**:
- Sequentially executes each compiled program
- Captures output for analysis
- Provides consistent test environment
- Facilitates observation and comparison

### `observation.txt`
**Purpose**: Documentation of experimental results and analysis.

**Contents**:
- Runtime observations from each program
- Analysis of process behavior and timing
- Notes on scheduling patterns
- Answers to lab questions
- Performance measurements

## How to Build

### Compile All Programs
```bash
make
```

### Compile Individual Programs
```bash
gcc -o 1 1.c     # Compile 1.c to executable named "1"
gcc -o 3 3.c     # Compile 3.c to executable named "3"
# ... and so on
```

### Using the Makefile
```bash
make all         # Build everything (default)
make 1           # Build only program 1
make 3           # Build only program 3
make clean       # Remove all executables
```

## How to Run

### Execute Individual Programs
```bash
./1              # Run program 1
./3              # Run program 3
./5              # Run program 5
# ... etc.
```

### Run All Programs with Script
```bash
chmod +x lab2.sh
./lab2.sh
```

### Observe Process Behavior
Use Unix tools to monitor processes:
```bash
ps aux | grep username     # View running processes
top                        # Monitor process activity
pstree -p                  # Visualize process tree
```

## Expected Behavior and Output

### Normal Execution Pattern
- Each program prints diagnostic messages showing parent and child execution
- Output interleaving varies due to CPU scheduling
- Programs with `wait()` show deterministic ordering
- Programs without `wait()` show race conditions

### Common Observations
- Child processes may complete before or after parent
- Delay values significantly affect output interleaving
- Larger delays create more context switches
- Smaller delays may batch executions together
- System load affects scheduling patterns

## Key Concepts and Learning Outcomes

### Process Creation with `fork()`
```c
pid_t pid = fork();
if (pid < 0) {
    // Error handling
    fprintf(stderr, "fork failed\n");
    exit(1);
} else if (pid == 0) {
    // Child process code
    printf("I am the child\n");
} else {
    // Parent process code
    printf("I am the parent, child PID = %d\n", pid);
}
```

### Process Synchronization with `wait()`
```c
pid_t child_pid = fork();
if (child_pid > 0) {
    // Parent waits for child to finish
    wait(NULL);              // Simple wait, ignore exit status
    // or
    int status;
    waitpid(child_pid, &status, 0);  // Wait for specific child
}
```

### Timing and Delays
- `sleep(seconds)` - Second-level delays
- `usleep(microseconds)` - Microsecond-level delays
- `nanosleep()` - Nanosecond-level delays (most precise)

### Process States
1. **Running**: Currently executing on CPU
2. **Ready**: Waiting for CPU time
3. **Blocked**: Waiting for I/O or event
4. **Zombie**: Terminated but not reaped
5. **Orphan**: Parent terminated before child

### Zombie Processes
**What is a zombie?**
- Process that has terminated but still has entry in process table
- Parent hasn't called `wait()` to read exit status
- Shows as `<defunct>` in process listings
- Consumes system resources (process table entries)

**Prevention**:
- Always call `wait()` or `waitpid()` in parent
- Use signal handlers (`SIGCHLD`) for asynchronous cleanup
- Ensure parent outlives children or properly reaps them

## What I Learned
- I learned how to use `fork` safely, check for errors, and differentiate parent vs. child paths.
- I learned to use `wait`/`waitpid` as synchronization primitives to prevent zombie processes.
- I learned to inject artificial delays to observe CPU scheduling and context switches.
- I learned that wrapping all of these experiments in a `Makefile` keeps the iteration loop tight.
- I learned how CPU scheduling creates non-deterministic execution patterns.
- I learned the importance of proper process cleanup to prevent resource leaks.
- I learned to use indentation and formatting to visually track process execution.
- I learned how timing granularity (seconds vs. microseconds) affects observable behavior.

## Common Issues and Troubleshooting

### Compilation Errors
**Problem**: `undefined reference to 'wait'`
**Solution**: Ensure you include proper headers:
```c
#include <sys/wait.h>    // For wait()
#include <unistd.h>      // For fork(), usleep()
```

### Zombie Processes Lingering
**Problem**: `ps aux` shows `<defunct>` processes
**Solution**:
1. Ensure parent calls `wait()`
2. Kill parent process: `kill -9 <parent_pid>`
3. Zombies will be cleaned up by init

### Inconsistent Output
**Problem**: Output order changes on each run
**Solution**: This is expected! Process scheduling is non-deterministic. Use delays and `wait()` for more predictable ordering.

### usleep() Not Working
**Problem**: Delays don't seem to work on some systems
**Solution**:
- Check for `usleep()` deprecation warnings
- Use `nanosleep()` as modern alternative:
```c
struct timespec ts = {0, 3000000};  // 3ms
nanosleep(&ts, NULL);
```

### Make Command Not Found
**Problem**: `make: command not found`
**Solution**:
```bash
# macOS
xcode-select --install

# Ubuntu/Debian
sudo apt-get install build-essential

# CentOS/RHEL
sudo yum groupinstall "Development Tools"
```

## Further Exploration
- Modify delays to see how scheduling changes
- Create deeper process trees (parent → child → grandchild)
- Implement process communication with shared memory
- Use `exec()` family to replace process images
- Measure actual execution time with `time` command
- Create a process pool pattern for parallel work
- Implement signal handlers for graceful termination
- Explore `fork()` + `exec()` patterns (foundation for shells)
- Compare process creation overhead with different fork counts
