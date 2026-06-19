# 00-01 — Prerequisites

Complete every item before opening the first lesson. This is not optional.

---

## Read First: Theory Resources

### 1. Lode's Raycasting Tutorial (MANDATORY)

**URL:** <https://lodev.org/cgtutor/raycasting.html>

Work through Parts 1–3:

- Part 1: Basic DDA raycasting with flat-colored walls
- Part 2: Floor and ceiling casting
- Part 3: Sprite casting (needed for Module 08)

**What to extract:** Understand `deltaDistX`, `sideDistX`, `perpWallDist`, and why we use the camera plane. You will implement these formulas yourself. Read, do not copy.

### 2. Permadi's Tutorial (RECOMMENDED)

**URL:** <https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/>

This gives the visual intuition that Lode's tutorial assumes. Permadi explains:

- How a ray hits a wall geometrically
- The fish-eye problem with a diagram
- The relationship between viewing angle and distance

Read sections 1–9 for the geometric foundation.

### 3. MiniLibX Documentation

**URL:** <https://harm-smits.github.io/42docs/libs/minilibx>

Reference for all MLX function signatures, hook event codes, and image buffer usage. Keep this open while working on Module 01.

**macOS key event codes** you will need:

```c
#define KEY_ESC     53
#define KEY_W       13
#define KEY_A        0
#define KEY_S        1
#define KEY_D        2
#define KEY_LEFT    123
#define KEY_RIGHT   124
#define KEY_UP      126
#define KEY_DOWN    125
```

### 4. Aurelien Brabant MLX Guide (RECOMMENDED)

**URL:** <https://aurelienbrabant.fr/blog/getting-started-with-the-minilibx>

Explains the offscreen image buffer pattern that is the core rendering technique for this project. After reading, you should understand why `mlx_pixel_put` is wrong and `mlx_put_image_to_window` on an offscreen buffer is right.

### 5. Game Engine Black Book: Wolfenstein 3D (OPTIONAL but inspiring)

**URL:** <https://fabiensanglard.net/gebbwolf3d/>

The Game Engine Black Book documents how John Carmack built the raycasting engine in Wolfenstein 3D. Chapter 5 is particularly relevant:

- Why integer/fixed-point arithmetic was used instead of floats (speed)
- How texture mapping was implemented
- The optical tricks used to create depth

> *"The bottleneck was cycles. Every float operation was expensive. We had to be very careful about what we computed and when."* — Fabien Sanglard summarizing Carmack

In cub3D we use floats because modern CPUs have hardware FPUs, but the **underlying principle is the same**: minimize per-pixel computation. This is why your floor/ceiling can be a solid color instead of ray-traced per pixel.

---

## Install Checklist

- [ ] MiniLibX is installed and `#include <mlx.h>` compiles
- [ ] `gcc` with `-lmlx -framework OpenGL -framework AppKit` links without errors (macOS) OR `-lmlx -lX11 -lXext` (Linux)
- [ ] `norminette` is installed and accessible from the terminal
- [ ] `valgrind` or `leaks` is available for memory checking
- [ ] Both partners have cloned this repository

---

## Code References to Have Open (Do NOT copy)

These repositories are for architectural study only. Clone them locally for reference:

```bash
# Keep in a separate folder, never inside your cub3d project
git clone https://github.com/derfleck/cub3d.git       ~/refs/derfleck
git clone https://github.com/mathias-mrsn/cub3d.git   ~/refs/mathias-mrsn
```

Pre-extracted annotated snippets from these repos live in `code_snippets/`. Use those instead of reading the full repos directly while studying.
