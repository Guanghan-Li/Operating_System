# Lab 3 – Pipes and Exec

## Overview
This lab demonstrates inter-process communication (IPC) through Unix pipes and process replacement via the `exec` family. Building on Lab 2's process creation fundamentals, you'll learn how to chain processes together, redirect standard input/output streams, and orchestrate complex data flows between programs. These techniques are the foundation of shell pipelines and how Unix composes small tools into powerful workflows.

## Purpose
Chain multiple Unix programs together via pipes so I can see how standard input/output redirection enables inter-process communication in C.

## Prerequisites
- C compiler (gcc or clang)
- Completed Lab 2 (process creation basics)
- Understanding of file descriptors (stdin=0, stdout=1, stderr=2)
- Familiarity with Unix utilities (ls, cat, grep, wc, sort)
- Knowledge of process lifecycle and `fork()`/`wait()`

## Core Concepts

### Pipes
An **anonymous pipe** is a unidirectional communication channel between processes:
- Created with `pipe(int fds[2])`
- `fds[0]` = read end
- `fds[1]` = write end
- Data written to write end can be read from read end
- Kernel buffers data automatically
- Blocking semantics: readers wait if empty, writers wait if full

### File Descriptor Redirection
- `dup2(oldfd, newfd)` duplicates file descriptor
- Common pattern: redirect stdout to pipe write end
- Example: `dup2(pipe_write, 1)` makes stdout write to pipe
- After redirection, must close original descriptors to prevent leaks

### Process Replacement with exec
- `exec` family replaces current process image with new program
- Variants: `execl`, `execlp`, `execv`, `execvp`, `execve`
- File descriptors remain open across `exec` (unless marked close-on-exec)
- Combined with `fork()`, enables running external programs with custom I/O

## Files in This Lab

### `1.c` - Basic Pipe Communication
**Purpose**: Simplest pipe example with parent-child communication.

**What it does**:
- Parent creates pipe and forks child
- Parent writes message to pipe
- Child reads message from pipe
- Demonstrates basic pipe setup and usage

**Key concepts**:
- `pipe()` system call
- Read/write end management
- Basic IPC pattern
- Closing unused pipe ends

### `2.c` - Pipe with Output Redirection
**Purpose**: Redirect child's stdout to pipe.

**What it does**:
- Parent creates pipe before forking
- Child redirects stdout to pipe write end using `dup2()`
- Child prints normally (goes to pipe instead of terminal)
- Parent reads from pipe and displays

**Key concepts**:
- `dup2()` for file descriptor redirection
- Transparent output capture
- Process separation of concerns
- Standard stream manipulation

### `3.c` - Chaining Two Commands
**Purpose**: Simple pipeline of two Unix commands.

**What it does**:
- Implements basic pipeline like `ls | wc`
- First child runs `ls` with stdout to pipe
- Second child runs `wc` with stdin from pipe
- Parent coordinates both children

**Key concepts**:
- Two-process pipeline
- Bidirectional coordination
- Command chaining
- Multiple child management

### `4.c` - cat | grep Pipeline
**Purpose**: Classic text filtering pipeline.

**What it does**:
- Implements `cat /etc/passwd | grep root`
- First child: `cat /etc/passwd` → stdout to pipe
- Second child: `grep root` ← stdin from pipe
- Parent manages both processes and pipe

**Key concepts**:
- Text stream processing
- Filter pattern (producer-consumer)
- Real-world pipeline use case
- Coordinated file descriptor management

**Sample output**:
```
root:*:0:0:System Administrator:/var/root:/bin/sh
daemon:*:1:1:System Services:/var/root:/usr/bin/false
_cvmsroot:*:212:212:CVMS Root:/var/empty:/usr/bin/false
```

### `5.c` - Three-Process Pipeline
**Purpose**: Extended pipeline with three stages.

**What it does**:
- Creates chain of three processes with two pipes
- Example pattern: `cmd1 | cmd2 | cmd3`
- Demonstrates multi-stage data transformation
- Shows scaling beyond two processes

**Key concepts**:
- Multiple pipes in sequence
- Pipeline scalability
- Complex file descriptor juggling
- N-stage processing

### `6.c` - Pipe with Error Handling
**Purpose**: Robust pipe implementation with comprehensive error checking.

**What it does**:
- Adds error checking for all system calls
- Handles failure cases gracefully
- Demonstrates production-quality code
- Shows proper resource cleanup

**Key concepts**:
- Error handling patterns
- Resource leak prevention
- Defensive programming
- Production best practices

### `7.c` - Bidirectional Communication Setup
**Purpose**: Explores more complex communication patterns.

**What it does**:
- Sets up multiple pipes for different communication directions
- Demonstrates parent-child request-response pattern
- Shows advanced IPC architectures

**Key concepts**:
- Bidirectional IPC
- Request-response pattern
- Multiple concurrent pipes
- Complex process coordination

### `8.c` - Advanced Pipeline Patterns
**Purpose**: Comprehensive demonstration of pipeline techniques.

**What it does**:
- Combines multiple concepts from previous programs
- Demonstrates real-world pipeline scenarios
- Shows best practices and common patterns
- Serves as reference implementation

**Key concepts**:
- Pipeline design patterns
- Composite workflows
- Practical IPC applications
- Integration of fork/pipe/exec/wait

## How to Build

### Compile All Programs
```bash
gcc -o 1 1.c
gcc -o 2 2.c
gcc -o 3 3.c
gcc -o 4 4.c
gcc -o 5 5.c
gcc -o 6 6.c
gcc -o 7 7.c
gcc -o 8 8.c
```

### Or Use a Makefile
If a Makefile exists:
```bash
make all
make clean    # Remove executables
```

## How to Run

### Execute Individual Programs
```bash
./1    # Basic pipe demo
./2    # Pipe with redirection
./3    # ls | wc
./4    # cat | grep
./5    # Three-stage pipeline
./6    # With error handling
./7    # Bidirectional communication
./8    # Advanced patterns
```

### Compare with Shell Equivalents
```bash
# Shell version
cat /etc/passwd | grep root

# Our C implementation
./4
```

### Debug with strace
Monitor system calls to see pipes in action:
```bash
strace -f ./4 2>&1 | grep -E "pipe|fork|dup2|exec"
```

## Expected Behavior

### Normal Operation
- Programs execute silently or produce expected output
- Child processes complete before parent exits
- No zombie or orphan processes left behind
- Pipe data flows correctly between processes

### Common Output Patterns
- `./1`: Displays message passed through pipe
- `./2`: Shows redirected output captured via pipe
- `./3`: Shows file count from `ls | wc`
- `./4`: Displays lines containing "root" from /etc/passwd
- Programs may show diagnostic messages during execution

## Key Concepts and Learning Outcomes

### Creating a Pipe
```c
int fds[2];
if (pipe(fds) == -1) {
    perror("pipe failed");
    exit(1);
}
// fds[0] = read end
// fds[1] = write end
```

### Redirecting stdout to Pipe
```c
// In child process
dup2(pipe_write_end, STDOUT_FILENO);  // stdout → pipe
close(pipe_write_end);                // close original descriptor
close(pipe_read_end);                 // child doesn't read
execlp("ls", "ls", NULL);             // ls output goes to pipe
```

### Redirecting stdin from Pipe
```c
// In another child process
dup2(pipe_read_end, STDIN_FILENO);    // stdin ← pipe
close(pipe_read_end);                 // close original descriptor
close(pipe_write_end);                // child doesn't write
execlp("wc", "wc", NULL);             // wc reads from pipe
```

### Complete Two-Stage Pipeline Pattern
```c
int fds[2];
pipe(fds);

// Child 1: producer (stdout → pipe)
if (fork() == 0) {
    dup2(fds[1], 1);       // redirect stdout
    close(fds[0]);         // close unused read end
    close(fds[1]);         // close after dup
    execlp("ls", "ls", NULL);
}

// Child 2: consumer (stdin ← pipe)
if (fork() == 0) {
    dup2(fds[0], 0);       // redirect stdin
    close(fds[1]);         // close unused write end
    close(fds[0]);         // close after dup
    execlp("wc", "wc", "-l", NULL);
}

// Parent: close pipe and wait
close(fds[0]);
close(fds[1]);
wait(NULL);
wait(NULL);
```

### Critical Pipe Rules
1. **Close unused ends**: Each process must close pipe ends it doesn't use
2. **Close after dup2**: After duplicating, close the original descriptor
3. **Parent must close**: Parent must close both ends after forking
4. **Wait for children**: Always reap child processes to prevent zombies
5. **Order matters**: Set up redirections before `exec()`

### Why Closing Matters
- Pipes use reference counting
- Write end not closed → reader waits forever (deadlock)
- Read end not closed → writer gets SIGPIPE on exit
- File descriptor leaks waste system resources
- Proper cleanup ensures deterministic behavior

## What I Learned
- I learned how to set up `pipe` and redirect descriptors with `dup2` before calling `exec`.
- I learned to think about which ends of the pipe each process must close to avoid deadlock.
- I learned to orchestrate multiple children from a single parent and reap them deterministically.
- I learned that complex shell pipelines are just a few C system calls under the hood.
- I learned the importance of file descriptor management and preventing resource leaks.
- I learned how Unix composes small programs into complex workflows through IPC.
- I learned the producer-consumer pattern as implemented through pipes.
- I learned how shells implement command pipelines using fork, pipe, dup2, and exec.

## Common Issues and Troubleshooting

### Program Hangs / Deadlock
**Problem**: Program never completes, appears frozen
**Solution**: Check that all unused pipe ends are closed
```c
// Bad: forgot to close write end in reader
dup2(fds[0], 0);
close(fds[0]);
// Missing: close(fds[1]);  ← This causes deadlock!

// Good: close both ends appropriately
dup2(fds[0], 0);
close(fds[0]);
close(fds[1]);  ← Must close this too!
```

### Broken Pipe / SIGPIPE
**Problem**: Process receives SIGPIPE and terminates
**Solution**: Ensure reader exists and read end is open when writer executes

### No Output Appears
**Problem**: Expected output doesn't show
**Solution**:
- Verify `dup2()` is called before `exec()`
- Check that correct file descriptors are used (0=stdin, 1=stdout)
- Ensure parent closes pipe ends (output may be buffered)

### exec() Fails
**Problem**: `execlp: No such file or directory`
**Solution**:
- Use `execlp()` for PATH lookup or `execl()` with full path
- Check command spelling
- Add error checking: `perror("exec failed"); exit(1);` after exec

### File Descriptor Leaks
**Problem**: System runs out of file descriptors
**Solution**: Always close pipe ends after `dup2()` and in unused processes

### Zombie Processes
**Problem**: `ps` shows `<defunct>` processes
**Solution**: Parent must call `wait()` or `waitpid()` for each child

## Debugging Tips

### Trace System Calls
```bash
strace -f -e pipe,dup2,fork,exec ./4
```

### Check Open File Descriptors
```bash
lsof -p <pid>        # List open files for process
ls -l /proc/<pid>/fd # Linux: view file descriptors
```

### Add Diagnostic Output
```c
fprintf(stderr, "Child PID=%d: about to exec\n", getpid());
// stderr still goes to terminal even with stdout redirected
```

## Further Exploration
- Implement `ls -l | grep \.c | wc -l` in C
- Create a mini-shell that parses and executes pipelines
- Add support for stderr redirection with `2>`
- Implement tee functionality (write to file and stdout)
- Explore named pipes (FIFOs) for unrelated process communication
- Build a pipeline with 5+ stages to stress-test your understanding
- Implement bidirectional pipes for request-response patterns
- Add timeout mechanisms for handling slow pipeline stages
- Experiment with large data transfers to observe pipe buffer limits
- Compare pipe performance vs. other IPC mechanisms (shared memory, sockets)
