# Operating Systems Labs

This repository collects hands-on lab work for an Operating Systems course.  Each `LabN/` directory contains one or more short C programs or shell scripts that explore kernels, processes, and synchronization primitives.  The labs are intentionally small so you can focus on a single concept at a time and iterate quickly.

## Layout
- `Lab1/` — introductory shell exercises and simple observations scripted in Bash.
- `Lab2/` & `Lab3/` — collections of numbered C programs (e.g., `1.c`, `2.c`, …) that build intuition around processes, memory, and scheduling.
- `Lab4/` — adds a `Makefile`, a convenience runner (`lab4.sh`), and `observation.txt` for recording findings.
- `Lab5/` — expands into multithreading experiments such as `threadSync.c`, again backed by a `Makefile`.
- `Lab7/` — reserved for future Operating Systems labs; keep the folder so git tracks the placeholder.

Feel free to add new lab folders following the same naming convention.

## Building and Running
1. **Direct compile** — for a single translation unit run:
   ```bash
   gcc -Wall -Wextra -std=c17 Lab3/4.c -o Lab3/4
   ./Lab3/4
   ```
2. **Via Makefile** — when a lab provides build rules (e.g., `Lab4` or `Lab5`):
   ```bash
   make -C Lab4        # builds every numbered step
   make -C Lab4 clean  # removes the generated binaries
   ```
3. **Shell helpers** — `Lab4/lab4.sh` demonstrates how to chain the compiled steps together for quick verification.  You can mirror this pattern for other labs.

## Keeping the Repo Clean
- Compiled binaries live next to their sources (e.g., `Lab4/1` next to `Lab4/1.c`).  Run `make clean` or delete the binaries before committing, or rely on the new `.gitignore` to prevent accidental staging.
- The root `.gitignore` filters macOS metadata, object files, static and shared libraries, and the extensionless executables produced by each lab.
- If you add executables with new names, either update the relevant `Makefile` to emit them into a `bin/` directory or extend `.gitignore` with the new pattern so only source stays under version control.

## Extending the Labs
When creating another experiment:
1. Create `LabN/` (or add files to an existing lab).
2. Drop in your `.c` sources and, if needed, a short `Makefile` or shell script explaining how to build and test.
3. Document noteworthy behavior in a companion `README.md` or `observation.txt` inside the lab so future-you remembers what to look for.

Everything here is focused on Operating Systems concepts—no CSEN 177 boilerplate required.  Keep iterating, break things safely, and jot down what you discover along the way.
