# 05 — 2D Grid View: Drawing the Map, Player, and Direction

## Outcome
You have a working 2D top-down debug canvas that draws the parsed map as colored cells, places the player dot at the correct float position, renders a facing-direction arrow, and responds to WASD and arrow-key input in real time.

**This is the final Foundation milestone.** If this works, the transition to a 3D raycaster is a mathematical extension, not a blind leap.

---

## 1. Why This Canvas Matters

When your 3D raycaster produces wrong output, you will have no way to distinguish:
- Wrong DDA step direction
- Wrong side hit detection
- Wrong camera-plane calculation
- Wrong ray direction per column

The 2D canvas lets you draw one ray at a time and watch it hit exactly the cell it should. It removes ambiguity. **Build this before touching 3D.**

---

## 2. Architecture: Reuse Your Buffer

Your 2D canvas is just another draw pass into the same `render_image` buffer you built in Lesson 2. No new MLX concepts:

```
clear render_image buffer
→ draw_grid()          ← iterate over map cells, draw rect per cell
→ draw_player()        ← draw a filled square at float pos
→ draw_facing_dir()    ← draw a line segment from player following dir vector
→ draw_single_ray()    ← optional: draw one ray in color until it hits a wall
→ mlx_put_image_to_window()
```

---

## 3. Cell-to-Screen Mapping (so_long Pattern)

This is the same formula from Lesson 4, now actually used for drawing:

```c
// compute once at init, store in game struct:
int cell_dim = min(canvas_w / map_w, canvas_h / map_h);
int offset_x = (canvas_w - cell_dim * map_w) / 2;
int offset_y = (canvas_h - cell_dim * map_h) / 2;

// top-left pixel of cell (gx, gy):
int sx = offset_x + gx * cell_dim;
int sy = offset_y + gy * cell_dim;
```

Draw the cell as a filled rectangle of `cell_dim × cell_dim` pixels.

---

## 4. Color Scheme for the Grid

| Cell type | Color |
|-----------|-------|
| `1` — wall | `0x444444` (dark grey) |
| `0` — floor | `0x1a1a2e` (near-black blue) |
| ` ` — void | `0x000000` (black) |
| Spawn cell | `0x004400` (dark green) — only at boot |
| Grid lines | 1-pixel border, `0x333333` |

Draw each wall with a 1-pixel-inset border so cells have visible separation.

---

## 5. Drawing the Player

The player is a float position `(pos_x, pos_y)` in grid coordinates. Convert to screen:

```c
// Player screen position:
int px = offset_x + (int)(pos_x * cell_dim);
int py = offset_y + (int)(pos_y * cell_dim);

// Draw a filled square centered on (px, py):
draw_rect(&img, px - 4, py - 4, 8, 8, 0xFFFFFF);
```

Use a bright color so it's visible against the cell grid. White or cyan both work.

---

## 6. Drawing the Facing Direction Arrow

The direction vector `(dir_x, dir_y)` is a unit vector. Scale it to screen pixels:

```c
#define DIR_LENGTH 25  // pixels

int ex = px + (int)(dir_x * DIR_LENGTH);
int ey = py + (int)(dir_y * DIR_LENGTH);

draw_line(&img, px, py, ex, ey, 0xFF4444);  // red arrow
```

`draw_line` uses Bresenham's algorithm (or the simple DDA approach):

```c
void draw_line(t_image *img, int x0, int y0, int x1, int y1, unsigned int color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        put_pixel(img, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
}
```

---

## 7. Drawing a Single Debug Ray

As a final 2D debug step, cast one ray — the ray for screen column `W/2` (center of screen) — and draw it in yellow until it hits a wall:

```c
// Center ray calculation (same formula as full raycaster, just one column)
double ray_dir_x = dir_x;  // cameraX = 0 at center column
double ray_dir_y = dir_y;

// step through the grid with DDA, accumulate screen-space line
// Stop when map[grid_y][grid_x] == '1'
// Draw each step as a pixel in 0xFFFF00 (yellow)
```

This lets you visually verify:
- The ray starts at the player
- It travels in `dir` direction
- It terminates at a wall cell, not beyond it

When you rotate the player, the yellow ray should rotate. When you move forward, the ray should get shorter or longer depending on what's ahead.

---

## 8. Connecting to from so_long

Your so_long did all this implicitly:

| so_long component | 2D canvas equivalent |
|-------------------|----------------------|
| `draw_map()` → `draw_cell()` per `(x, y)` | `draw_grid()` per grid cell |
| `offset_x`, `offset_y`, `cell_dim` | Same formula, same variables |
| `player_visual_x`, `player_visual_y` as floats | `pos_x`, `pos_y` float position |
| Enemy direction marker | Player direction arrow |

The main added concept is the **facing direction arrow** and the **single-ray DDA** — both of which your `so_long` didn't need because it was top-down.

---

## 9. Build Exercise

Create `examples/03_grid_view/main.c`. It must:

1. Parse `../tests/valid/01_minimal_N.cub` (hardcode the path for now)
2. Compute `cell_dim`, `offset_x`, `offset_y`
3. Each frame:
   - Clear buffer
   - Draw grid cells with colors per type
   - Draw player as 8×8 white square
   - Draw red direction arrow 25px long
4. WASD moves the player (float position, check collision against grid)
5. LEFT/RIGHT rotates `dir` and `plane`
6. ESC exits cleanly

**Stretch**: Draw the center debug ray in yellow and watch it hit the correct wall cell.

---

## Checkpoint (Module 01 Final Exit Criteria)

All of the following must be true before moving to Module 2:

- [ ] Map cells draw in correct colors at correct positions
- [ ] Grid is centered using `offset_x` / `offset_y`
- [ ] Player dot is white and positioned at float `(pos_x, pos_y)`, not integer
- [ ] Red direction arrow always points where the player faces
- [ ] WASD moves the player with collision detection
- [ ] LEFT/RIGHT rotates both `dir` and `plane` together
- [ ] Rotating 360° leaves the vectors identical (no drift)
- [ ] *(Stretch)* Yellow center ray terminates at the correct wall cell

---

## Crisis Card

| Symptom | Fix |
|---------|-----|
| Grid is squished or stretched | `cell_dim` was computed from only width or only height; take the `min` of both |
| Player dot is off-center from its cell | You're placing at `gx * cell_dim` but forgetting `offset_x` |
| Direction arrow goes the wrong way | Your initial `dir_y` for `N` should be `-1` (Y-down coordinate space) |
| Rotating reverses after 90° | You forgot to save `old_dir_x` / `old_plane_x` before overwriting them in the rotation |
| Arrow has wrong length | Normalize `dir` before scaling — it should already be unit length if you initialized correctly |
| Segfault when player reaches map edge | Your collision isn't checking bounds before indexing `grid[y][x]` |

---

## Review Questions for Both Partners

1. If the player is at `(3.7, 2.1)`, which grid cell are they in? What is `(int)3.7`?
2. What does the direction vector look like if the player spawns with `W`? Draw it.
3. After rotating left by 45°, what happens to the camera plane vector?
4. Why does the center-ray DDA help you verify the raycaster before going 3D?
5. What's the difference between `player_visual_x` (so_long) and `pos_x` (cub3d)?

## Journal Prompt

Today is the last day of Foundation. Write in your journal:
- Describe the 2D canvas in one paragraph as if explaining it to an evaluator
- Paste the output of `./examples/03_grid_view` in your notes or take a screenshot
- What one bug took the most time to fix today?
- Are both partners ready to explain cell-to-screen mapping out loud?
