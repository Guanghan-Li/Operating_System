# Lab 1 – Shell Scripting Warmup

## Overview
This lab provides a hands-on introduction to POSIX shell scripting for automating common Operating Systems workflows. Through practical examples, you'll learn to inspect the execution environment, validate user input, perform arithmetic operations, and orchestrate Unix utilities into cohesive scripts. This foundation is critical for system administration, build automation, and reproducible experiments throughout the course.

## Purpose
Automate simple command-line workflows using POSIX shell so that common Operating Systems experiments (environment inspection, input validation, iterative calculations) can be reproduced without manually retyping every command.

## Prerequisites
- POSIX-compliant shell (bash, sh, zsh)
- Basic Unix utilities: `ls`, `wc`, `whoami`, `who`, `expr`, `bc`
- Text editor for viewing/modifying scripts
- Unix/Linux/macOS environment

## Files in This Lab

### `lab1s1.sh`
**Purpose**: Comprehensive demonstration of environment inspection, user validation, and interactive loop-based calculations.

**What it does**:
1. Displays script execution path and current directory statistics
2. Prints environment variables: `HOME`, `USER`, `PATH`, `PWD`, process ID
3. Validates that the logged-in user is "garrick" (modify for your username)
4. Interactively calculates rectangle areas in a loop until user responds "No"

**Key concepts demonstrated**:
- Environment variable access
- Command substitution with `$()` and backticks
- Process information (`$$`, `whoami`, `who`)
- Conditional statements (`if-then-else`)
- While loops with user-controlled exit conditions
- Input validation and arithmetic with `expr`

**Sample execution**:
```bash
chmod +x lab1s1.sh
./lab1s1.sh
```

### `lab1s3.sh`
**Purpose**: Mathematical computation using Unix pipelines and the `bc` calculator.

**What it does**:
1. Prompts the user for a circle radius
2. Computes area using the formula: π × r²
3. Computes perimeter (circumference) using: 2 × π × r
4. Uses `bc` for floating-point arithmetic with configurable precision

**Key concepts demonstrated**:
- Pipeline composition and data flow
- Floating-point arithmetic via `bc`
- Setting precision with `scale` in `bc`
- Reading user input with `read`
- Command substitution for inline calculations

**Sample execution**:
```bash
chmod +x lab1s3.sh
./lab1s3.sh
# Enter radius: 5
# Area: 78.50
# Perimeter: 31.40
```

### `lab1s2.txt`
**Purpose**: Text file used for documentation or observation notes from Step 2 exercises.

**Contents**: Likely contains sample outputs, test results, or answers to lab questions about script behavior.

## How to Run

### Making Scripts Executable
Before first use, grant execute permissions:
```bash
chmod +x lab1s1.sh lab1s3.sh
```

### Running Individual Scripts
Execute each script directly:
```bash
./lab1s1.sh    # Environment inspection and rectangle calculator
./lab1s3.sh    # Circle area and perimeter calculator
```

### Expected Behavior

**For `lab1s1.sh`**:
- Displays file count and line counts in current directory
- Shows environment variables
- Checks username (exits if not "garrick" or configured user)
- Prompts repeatedly for rectangle dimensions
- Calculates and displays area
- Continues until user types "No"

**For `lab1s3.sh`**:
- Prompts for radius input
- Outputs circle area with 2 decimal places
- Outputs perimeter with 2 decimal places
- Uses π ≈ 3.14159 for calculations

## Key Concepts and Learning Outcomes

### Environment Variables
- `$HOME`: User's home directory path
- `$USER`: Current username
- `$PATH`: Executable search path
- `$PWD`: Present working directory
- `$$`: Current process ID (PID)

### Control Flow
- **Conditionals**: `if [ condition ]; then ... else ... fi`
- **Loops**: `while [ condition ]; do ... done`
- **Exit codes**: `exit 0` (success), `exit 1` (failure)

### Command Substitution
Two equivalent syntaxes:
```bash
result=$(command)    # Modern, preferred
result=`command`     # Legacy, still works
```

### Arithmetic Operations
- **Integer arithmetic**: `expr` for basic operations
- **Floating-point**: `bc` with `scale` for precision
- **Example**: `echo "scale=2; 22/7" | bc` outputs `3.14`

### Input/Output
- **Reading input**: `read variable_name`
- **Printing output**: `echo "message"`
- **Piping data**: `command1 | command2`

### User Validation
- Check current user with `whoami`
- Count logged-in users with `who | wc -l`
- Implement access control with conditional exits

## What I Learned
- I learned how to capture environment variables (`HOME`, `PATH`, `PWD`) and process identifiers inside a script.
- I learned to guard a workflow with simple authentication/validation logic before the rest of the script runs.
- I learned to loop over user-driven calculations until a sentinel value is reached.
- I learned to combine command substitution with Unix tools (`wc`, `expr`, `bc`) to build larger behaviors from tiny reusable commands.
- I learned to make scripts executable and understand file permissions in Unix systems.
- I learned the difference between `expr` for integer arithmetic and `bc` for floating-point calculations.
- I learned proper shell script structure: shebang line, error handling, and clean exit codes.

## Common Issues and Troubleshooting

### Permission Denied
**Problem**: `./lab1s1.sh: Permission denied`
**Solution**: Make script executable with `chmod +x lab1s1.sh`

### User Validation Fails
**Problem**: Script exits with "Check who logged in!"
**Solution**: Edit line 18 in `lab1s1.sh` to match your username instead of "garrick"

### bc Command Not Found
**Problem**: `bc: command not found`
**Solution**: Install bc calculator:
```bash
# macOS
brew install bc

# Ubuntu/Debian
sudo apt-get install bc

# CentOS/RHEL
sudo yum install bc
```

### Wrong Shell Interpreter
**Problem**: Script behaves unexpectedly or has syntax errors
**Solution**: Ensure you're using a POSIX-compliant shell. Check shebang line is `#!/bin/sh` or `#!/bin/bash`

### Infinite Loop in lab1s1.sh
**Problem**: Script won't exit when typing "No"
**Solution**: Ensure you type exactly "No" (case-sensitive). Alternative: Press `Ctrl+C` to force exit.

## Further Exploration
- Modify `lab1s1.sh` to calculate triangle areas instead of rectangles
- Extend `lab1s3.sh` to compute sphere volume and surface area
- Add error handling for negative numbers or non-numeric input
- Create a menu system that lets users choose which calculation to perform
- Experiment with different `bc` precision settings (scale=5, scale=10)
- Add logging functionality to save results to a file
