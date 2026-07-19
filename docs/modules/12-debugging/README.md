# Module 12 — Debugging, Memory Safety & Performance

## What This Module Teaches

After completing earlier modules you have a working engine. But it crashes sometimes. Valgrind shows leaks you cannot find. And the FPS drops are mysterious.

This module teaches you the discipline of **self-sufficient debugging** — how to find and fix issues without looking at solutions, using the same tools professional C developers use every day.

## Contents

| File | Topic |
|------|-------|
| [`01-valgrind-gdb-profiling.md`](./01-valgrind-gdb-profiling.md) | Full workflow: Valgrind, LLDB/GDB, AddressSanitizer, FPS profiling |
| [`02-valgrind-cheat-sheet.md`](./02-valgrind-cheat-sheet.md) | Short cub3D commands + `clean_exit` teardown notes |

## Key Skills Learned

- Read and interpret a full Valgrind report
- Use `bt` (backtrace) in LLDB/GDB to pinpoint crash lines
- Compile with AddressSanitizer for instant crash context
- Add frame-time profiling to identify bottlenecks
- Apply the `NULL-initialize everything` pattern to prevent leaks structurally
- Use `top`/`htop` to distinguish algorithmic vs windowing performance issues
