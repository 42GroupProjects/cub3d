# 12 — Debugging, Leak Prevention & Performance Benchmarking

## Why This Module Exists

Every student at some point hits a wall with a bug they cannot find. Their program crashes at an address they cannot explain, Valgrind shows "100 bytes definitely lost" they cannot locate, or their game runs at 4 FPS and they do not know why.

This module teaches you how to **think like the tools** — not how to fix specific bugs, but how to systematically find and diagnose any bug yourself without looking at solutions.

---

## 01 — Valgrind: Zero Leak Methodology

### What Valgrind Is

Valgrind intercepts every `malloc`, `free`, `read`, and `write` your program executes. It tracks which memory is allocated and freed, and at program exit, reports anything that was allocated but never returned.

### Running Valgrind Correctly

```bash
# Basic leak check
valgrind --leak-check=full ./cub3d maps/valid.cub

# Full output: show EVERY leak source, not just summary
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cub3d maps/valid.cub

# Suppress MLX internal leaks (which you cannot control)
valgrind --leak-check=full --suppressions=mlx.supp ./cub3d maps/valid.cub
```

### Reading Valgrind Output

```
==12345== HEAP SUMMARY:
==12345==   in use at exit: 112 bytes in 3 blocks
==12345==   total heap usage: 47 allocs, 44 frees, 8,920 bytes allocated
==12345==
==12345== 24 bytes in 1 blocks are DEFINITELY LOST in loss record 1 of 3
==12345==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck.so)
==12345==    by 0x10927A: ft_strdup (ft_strdup.c:12)
==12345==    by 0x109B82: parse_no_header (parser.c:34)
==12345==    by 0x109F11: parse_file (parser.c:87)
```

**How to read this:**
1. The call stack reads **bottom-to-top** — start reading from `parse_file → parse_no_header → ft_strdup`
2. `parse_no_header` at line 34 in `parser.c` called `ft_strdup` and the returned pointer was never `free()`'d
3. Go to `parser.c:34` and trace where the returned string goes — is it stored? Is it freed on error paths?

### The Three Categories

| Category | Meaning | Action |
|----------|---------|--------|
| **Definitely Lost** | Allocated, pointer never freed or even accessible | **Fix immediately** — this is a real leak |
| **Indirectly Lost** | Lost because a parent struct holding the pointer was lost | **Fix the parent leak first** |
| **Still Reachable** | Pointer exists at exit but was never freed | Fix if it is your allocation; suppress if it is MLX internal |

### The Systematic Fix Protocol

When you see "100 bytes definitely lost":
1. Read the call stack — find the allocating function and line
2. Add a `printf("DEBUG: allocated at %s:%d\n", __FILE__, __LINE__)` right after the malloc
3. Add `printf("DEBUG: freed at %s:%d\n", __FILE__, __LINE__)` at every place you free it
4. Run the program — did the "freed" message print? If not, trace the code path that skips it
5. Add the missing `free()` on all early-exit error paths

### The MLX Suppression File

MLX on Linux has internal leaks the student cannot fix. Create `mlx.supp` to silence them:
```
{
   mlx_internal_leak
   Memcheck:Leak
   ...
   fun:mlx_init
}
```

Run with: `valgrind --suppressions=mlx.supp --leak-check=full ./cub3d map.cub`

---

## 02 — LLDB / GDB: Debugging Crashes

### When to Use a Debugger

- Program segfaults and you do not know at which line
- Program produces wrong output and you want to watch variable values in real time
- Program hangs indefinitely — you want to see where it is stuck

### Compiling for Debugging

Always compile with `-g` (debug symbols) and **without** `-O2` (optimization):
```makefile
CFLAGS = -Wall -Wextra -Werror -g
# Remove -O2 while debugging — it reorders code and confuses the debugger
```

### LLDB Quick Reference (macOS)

```bash
# Launch
lldb ./cub3d maps/valid.cub

# Core commands inside lldb:
run                          # Start the program
bt                           # Backtrace — show call stack AFTER a crash
frame select 3               # Jump to frame #3 in the backtrace
p my_variable                # Print a variable's value
p *game                      # Print entire struct
p game->map.grid[0]          # Print first row of map grid
p (int)strlen(game->map.grid[0])  # Evaluate an expression
b parser.c:87                # Set breakpoint at parser.c line 87
b parse_no_header            # Set breakpoint at function
c                            # Continue after breakpoint
n                            # Next line (step over)
s                            # Step into function call
q                            # Quit
```

### GDB Quick Reference (Ubuntu/Linux)

```bash
gdb ./cub3d
run maps/valid.cub

bt          # Backtrace on crash
frame 2     # Select frame
p variable  # Print
b file.c:42 # Breakpoint
c           # Continue
n           # Next line
s           # Step into
q           # Quit
```

### Reading a Backtrace

```
(lldb) bt
* thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS
  * frame #0: 0x00007fff libsystem_c.dylib`strlen + 16
    frame #1: 0x0000000100002a1c cub3d`ft_putendl_fd(s="", fd=2) at ft_putendl.c:14
    frame #2: 0x0000000100003f88 cub3d`parse_error(game=0x7ffeefbff4a0, msg=0x0) at parser.c:21
    frame #3: 0x000000010000411c cub3d`check_extension(path=0x0) at parser.c:56
```

Read top-to-bottom: `check_extension` (frame #3) called `parse_error` with `msg=0x0` (NULL), which called `ft_putendl_fd` with `s=""`, which crashed in `strlen` because the string pointer was ultimately NULL.

**The fix:** `parse_error` should guard against a NULL message.

### AddressSanitizer (Fastest Crash Locator)

Compile with AddressSanitizer for instant crash location **with full source context**, no debugger needed:

```makefile
# Add these flags temporarily during development
CFLAGS += -fsanitize=address -fsanitize=undefined
```

```bash
make && ./cub3d map.cub

# On crash, output looks like:
==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000000310
READ of size 4 at 0x602000000310
    #0 0x10b23c in get_texture_pixel src/render.c:145
    #1 0x10a891 in draw_wall src/render.c:87
```

Line numbers, source files, and the exact bad memory operation — all instantly, no `bt` commands required. This is the fastest debugging loop for development.

---

## 03 — Memory Leak Prevention Patterns

These are structural patterns that prevent leaks from being introduced in the first place.

### Pattern 1: The Centralized `clean_exit`

Never call `exit()` or `return` directly from parsing functions. Always pass back to a single exit point:

```c
void    clean_exit(t_game *game, int exit_code)
{
    int i;

    // Layer 1: MLX images
    i = 0;
    while (i < 4)
    {
        if (game->tex[i].img)
            mlx_destroy_image(game->mlx, game->tex[i].img);
        i++;
    }
    if (game->img.img)
        mlx_destroy_image(game->mlx, game->img.img);

    // Layer 2: MLX window/display
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    #ifdef __linux__
    if (game->mlx)
    {
        mlx_destroy_display(game->mlx);
        free(game->mlx);
    }
    #endif

    // Layer 3: Map and parsed data
    free_map(game->map.grid);
    free_texture_paths(game->tex_paths); // paths parsed before MLX init

    exit(exit_code);
}
```

### Pattern 2: NULL-Initialize Everything

Use `ft_memset` on your entire game struct before using any field. Any pointer not explicitly set remains `NULL`, making `if (ptr) free(ptr)` safe on all cleanup paths:

```c
int main(void)
{
    t_game game;

    ft_memset(&game, 0, sizeof(t_game)); // All pointers start NULL
    // Now clean_exit can safely check every pointer without uninit reads
}
```

### Pattern 3: The Ownership Rule

For every `malloc`, decide immediately: who owns this allocation and when will it be freed? Write a comment:

```c
// OWNED by: game->map.grid
// FREED in: clean_exit -> free_map()
game->map.grid = allocate_grid(height, width);
```

Ownerless allocations (where you forget who frees them) are the root cause of most leaks.

---

## 04 — FPS Benchmarking & Performance Profiling

### Adding an FPS Counter

The simplest performance indicator: track time between frames and display FPS in the window title.

```c
#include <time.h>

/* Norminette-compliant FPS display */
void    display_fps(t_game *game)
{
    static clock_t  last_time = 0;
    clock_t         now;
    double          delta;
    char            title[32];
    int             fps;

    now = clock();
    if (last_time == 0)
    {
        last_time = now;
        return ;
    }
    delta = (double)(now - last_time) / CLOCKS_PER_SEC;
    last_time = now;
    if (delta <= 0)
        return ;
    fps = (int)(1.0 / delta);
    ft_itoa_buf(fps, title); // your itoa writing into a buffer
    mlx_set_window_title(game->mlx, game->win, title);
}
```

Call `display_fps` at the top of every `game_loop` iteration.

### Frame Time Analysis

FPS is misleading for debugging — a spike from 60fps to 15fps for one frame feels like a stutter but FPS averages hide it. Measure **frame time in milliseconds** instead:

```c
void    profile_frame(t_game *game)
{
    static clock_t frame_start;
    clock_t        frame_end;
    double         frame_ms;

    frame_end = clock();
    frame_ms = (double)(frame_end - frame_start) * 1000.0 / CLOCKS_PER_SEC;
    if (frame_ms > 20.0) // Flag anything slower than 50fps target
        ft_printf("WARNING: Slow frame: %.2f ms\n", frame_ms);
    frame_start = clock(); // Reset for next frame
}
```

### Identifying Performance Bottlenecks

Wrap specific rendering phases with timing brackets:

```c
int game_loop(t_game *game)
{
    clock_t t0, t1, t2, t3;

    t0 = clock();
    cast_all_rays(game);        // DDA + Wall rendering
    t1 = clock();
    render_sprites(game);       // Painter's algorithm
    t2 = clock();
    flush_to_screen(game);      // mlx_put_image_to_window
    t3 = clock();

    // Print every 60 frames
    if (game->frame_count % 60 == 0)
    {
        ft_printf("DDA: %.2fms | Sprites: %.2fms | Flush: %.2fms\n",
            MS(t0, t1), MS(t1, t2), MS(t2, t3));
    }
    game->frame_count++;
    return (0);
}
```

Where `MS(a, b) = (double)(b - a) * 1000.0 / CLOCKS_PER_SEC`.

If DDA is `14ms` but Sprites is `0.2ms`, your bottleneck is the raycasting loop — not the entity system.

### Performance Targets

| Metric | Target | Warning |
|--------|--------|---------|
| Total frame time | < 16.7ms (60fps) | > 33ms (< 30fps) |
| DDA ray loop (800 cols) | < 5ms | > 10ms |
| Sprite rendering | < 2ms | > 5ms |
| `mlx_put_image_to_window` | < 2ms | > 8ms |
| Parser load time | < 50ms | > 200ms |

### The `htop` / `top` Method (Quick Sanity Check)

Without any code changes:
```bash
# Terminal 1
./cub3d map.cub

# Terminal 2
top -pid $(pgrep cub3d)  # macOS
htop -p $(pgrep cub3d)   # Ubuntu
```

If CPU usage is `>50%` for a simple scene, you have an algorithmic problem (likely `mlx_pixel_put` instead of buffer writing, or a naive stepping loop).

If CPU is `<5%` but FPS is still low, the bottleneck is the windowing system flush — investigate `mlx_put_image_to_window` timing.

---

## 05 — Systematic Debugging Checklist

When something goes wrong, work through this checklist before asking for help:

```
[ ] Compile with AddressSanitizer. Does it show a specific line?
[ ] Run Valgrind. Is there a "definitely lost" block pointing to the bug location?
[ ] Add ft_printf("REACHED line X\n") to narrow down which code path executed.
[ ] In LLDB/GDB: set a breakpoint before the crash, print all suspect variables.
[ ] Check: is every pointer you dereference confirmed non-NULL at that point?
[ ] Check: is every array index within the allocated bounds?
[ ] Check: does the buggy line even logically belong in that function?
[ ] Strip the program down: comment out subsystems until the bug disappears.
     The last thing you commented out is the guilty system.
[ ] Read the error output (crash address, errno, Valgrind call stack) character by character.
     The answer is usually literally written there.
```

---

## Crisis Card — Debugging Quick Lookup

| Symptom | Most Likely Cause | First Check |
|---------|------------------|-------------|
| Segfault at `strlen` | Passing NULL string to a print function | Trace backwards via `bt` — who passed NULL? |
| "Definitely lost" in Valgrind | `malloc` on one branch, `free` only on another | Ensure ALL exit paths free the pointer |
| Textures render scrambled | `line_length != width * 4` | Use `mlx_get_data_addr` value, not manual calculation |
| FPS drops to 4fps | `mlx_pixel_put` inside the render loop | Switch to RAM buffer + `mlx_put_image_to_window` |
| Game freezes when walking into a corner | AABB collision not independent per-axis | Split X and Y movement checks |
| Black windows on Ubuntu after minimize | Missing X11 Expose handler (event 12) | Register `mlx_hook(win, 12, 0, redraw, game)` |
| `asan: heap-buffer-overflow` on texture read | `offset = y * width * 4` instead of `line_length` | Always use real `line_length` in offset formula |
| Melee hits through walls | Z-Buffer / DDA occlusion check missing | Cast short ray toward enemy before confirming hit |
