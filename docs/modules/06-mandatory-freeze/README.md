# Module 06 — Mandatory Freeze

**Branch:** `main` | **Days:** 11–12

Do not touch Module 07, 08, or any bonus until every item in this module is complete.

---

## Files in This Module

| File | Contents |
|------|----------|
| [01-norm-compliance.md](./01-norm-compliance.md) | Super-struct pattern, splitting the DDA loop, ternary tricks |
| [02-valgrind-leaks-guide.md](./02-valgrind-leaks-guide.md) | Valgrind usage, reading call stacks, layered teardown |
| [03-error-robustness.md](./03-error-robustness.md) | Checklist of doom — every malformed input category |
| [04-integration-test.md](./04-integration-test.md) | **26-test battery**: file system, headers, maps, leaks, Norminette, oral defense |
| [05-capstone.md](./05-capstone.md) | Freeze commit, tag, and handoff to bonus track |

---

## Norm Pass

```bash
norminette src/ includes/
```

Common violations in raycasting code:
- Functions longer than 25 lines (split the DDA loop or texture draw loop)
- Variables not declared at top of block
- Lines over 80 characters (common in long math expressions)
- Global variables (forbidden — embed everything in `t_game`)
- Comma-separated variable declarations on same line

---

## Memory Leak Check

```bash
# macOS
leaks -atExit -- ./cub3d tests/mandatory_valid/01_minimal_N.cub

# Linux
valgrind --leak-check=full --show-leak-kinds=all \
    ./cub3d tests/mandatory_valid/01_minimal_N.cub
```

Run for each exit path:
1. Clean exit via ESC
2. Window X button exit
3. Error exit — run every file in `tests/mandatory_invalid/`

---

## Invalid Input Sweep

Run every file in `tests/mandatory_invalid/`. Each must:
- Print `Error\n` + a descriptive message
- Exit with non-zero status
- Not segfault
- Not leak memory

Tick all 15 boxes in `tests/evaluator_checklist.md`.

---

## The Final Refactor: Before and After

Most students end up with a 200–400 line `main.c` before they realize it needs splitting and Norming. Here is the pattern to refactor toward:

**Before (wrong):**

```c
// main.c — 300 lines, everything in one place
int main(int argc, char **argv)
{
    void *mlx;
    void *window;
    void *img;
    char *pixels;
    int bpp, ll, endian;
    // ... parse args ...
    // ... read file ...
    // ... parse headers ...
    // ... parse map ...
    // ... validate map ...
    // ... load textures ...
    // ... init player ...
    // ... set up hooks ...
    // ... mlx_loop here ...
    // game loop: cast ALL rays, draw ALL textures, handle ALL input
    return (0);
}
```

**After (correct — modular structure):**

```
src/
  main.c              ← arg check, init, mlx_loop() only
  core/
    init.c            ← mlx_init, window, image creation
    cleanup.c         ← destroy all in correct order
  parsing/
    parse_file.c      ← open file, route lines to header or map parser
    parse_header.c    ← NO/SO/WE/EA/F/C identifier parsing
    parse_map.c       ← map extraction, padding, spawn detection
    validate_map.c    ← character check, spawn count, wall enclosure
  engine/
    game_loop.c       ← dt calculation, key flag processing, render call
    raycast.c         ← column loop, DDA, perpWallDist
    textures.c        ← texture loading, wall_x, texX/Y sampling
    draw.c            ← put_pixel, draw_rect, floor/ceiling fill
  player/
    movement.c        ← walk, strafe, rotate
    input.c           ← key_down, key_up hooks
```

> **Why this matters for evaluation:** An evaluator may ask you to make a small change live (add debug output, change a constant, fix a bug they describe). If your raycasting logic is 200 lines in `main.c`, finding and changing the right place takes minutes. In the modular version, it's immediate.

---

## README Final Draft

```markdown
*A raycasting 3D engine built in C using MiniLibX, implementing a first-person perspective renderer for the cub3D 42 project.*

## Description

Cub3D implements a raycasting engine inspired by Wolfenstein 3D.
The engine renders textured walls, a solid floor and ceiling, and
supports WASD movement and left/right view rotation. The scene is
defined by a `.cub` configuration file.

## Build & Run

make
./cub3d maps/test.cub

make fclean    # clean all objects + binary

## Controls

| Key       | Action         |
|-----------|----------------|
| W         | Move forward   |
| S         | Move backward  |
| A         | Strafe left    |
| D         | Strafe right   |
| ← →       | Rotate view    |
| ESC       | Exit           |

## Map Format

The .cub file must have:
- NO/SO/WE/EA texture paths
- F and C RGB colors (0-255)
- A map made of 1 (wall), 0 (floor), and one spawn character (N/S/E/W)
- Spaces are valid void cells
- The map must be fully enclosed by walls

## Resources

- Lode's Raycasting Tutorial: https://lodev.org/cgtutor/raycasting.html
- MiniLibX docs: https://harm-smits.github.io/42docs/libs/minilibx
- Game Engine Black Book (Wolfenstein): https://fabiensanglard.net/gebbwolf3d/

## AI Usage

This project was built with AI assistance during documentation
and conceptual explanation. All AI-generated content was reviewed
and understood by both partners before implementation. See
docs/journal/ai_usage_log.md for the full record.
```

---

## Exit Checkpoint (Everything)

- [ ] `norminette` — zero errors on all `.c` and `.h` files
- [ ] No global variables anywhere
- [ ] `make` compiles with `-Wall -Wextra -Werror` with no warnings
- [ ] `make clean`, `fclean`, `re` all work correctly
- [ ] Leak check passes on clean exit, X-button exit, and all 15 invalid maps
- [ ] All 7 valid maps open correctly
- [ ] Both students pass the full oral defense drill without reading code
- [ ] README is final: italic first line, description, build, controls, map format, resources, AI section

---

## Oral Defense Drill

Run for both partners, with no notes access:

| Task | Time |
|------|------|
| Walk through parser: from `argv[1]` to valid `t_map` | 2 min |
| Explain DDA: deltaDistX, sideDistX, perpWallDist | 2 min |
| Explain texture selection (which 4-way branch, and why) | 1 min |
| Explain cleanup chain: what frees in what order | 1 min |
| Add live debug feature (e.g. print current pos to terminal) | 10 min |
| Introduce and then fix a deliberate bug (pixel trail) | 5 min |
| Justify one AI-assisted section | 2 min |
