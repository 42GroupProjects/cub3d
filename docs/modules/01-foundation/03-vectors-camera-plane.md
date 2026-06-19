# 03 — Vectors and Camera Plane

## Outcome
You understand the two vectors that drive all of Cub3D's rendering: the player's **direction vector** and the **camera plane**. You can set them from a spawn direction and rotate them correctly.

---

## 1. The Two Vectors You Need

Cub3D rendering lives entirely in 2D vector math. You never use angles directly during raycasting. Instead, you keep two 2D vectors that describe where the player is and where the camera looks.

| Vector | What it is | Example (facing East) |
|--------|-----------|----------------------|
| `dir` | Unit vector pointing in the player's facing direction | `(1, 0)` |
| `plane` | Camera plane: perpendicular to `dir`, scaled by FOV | `(0, 0.66)` |

The length of `plane` relative to `dir` sets the Field of View:
- `|plane| = 0.66` → ~66° FOV (the standard Lode tutorial value)

> [!IMPORTANT]
> `dir` and `plane` must always remain perpendicular. Rotating both together preserves
> this relationship. **Never change the length of `dir`** — it must remain a unit vector.

---

## 2. Setting Vectors from Spawn Direction

Your parser gives you a spawn direction character: `N`, `S`, `E`, `W`. Convert it:

```c
void init_player_direction(t_player *p, char dir)
{
    if (dir == 'N') { p->dir_x = 0;  p->dir_y = -1;
                      p->plane_x = 0.66; p->plane_y = 0; }
    if (dir == 'S') { p->dir_x = 0;  p->dir_y = 1;
                      p->plane_x = -0.66; p->plane_y = 0; }
    if (dir == 'E') { p->dir_x = 1;  p->dir_y = 0;
                      p->plane_x = 0;  p->plane_y = 0.66; }
    if (dir == 'W') { p->dir_x = -1; p->dir_y = 0;
                      p->plane_x = 0;  p->plane_y = -0.66; }
}
```

> [!NOTE]
> **MLX/C screen coordinates**: Y increases **downward**. So "North" on the map
> is `dir_y = -1`, not `+1`. This trips many people up.

---

## 3. Rotating the Vectors

To turn the player by `angle` radians per second, apply a 2D rotation matrix to both `dir` and `plane`. In practice, you compute a small rotation per frame using `rot_speed * dt`:

```c
void rotate_player(t_player *p, double angle)
{
    double old_dir_x   = p->dir_x;
    double old_plane_x = p->plane_x;

    // Rotate direction vector
    p->dir_x   = old_dir_x   * cos(angle) - p->dir_y   * sin(angle);
    p->dir_y   = old_dir_x   * sin(angle) + p->dir_y   * cos(angle);

    // Rotate camera plane by the same angle
    p->plane_x = old_plane_x * cos(angle) - p->plane_y * sin(angle);
    p->plane_y = old_plane_x * sin(angle) + p->plane_y * cos(angle);
}
```

When the left arrow is held: `rotate_player(p, +rot_speed * game->dt)`.  
When the right arrow is held: `rotate_player(p, -rot_speed * game->dt)`.

A good `rot_speed` is `2.0` (radians/second).

> [!TIP]
> You do not need `sin` and `cos` every frame in production. For small angles you can
> approximate: `sin(θ) ≈ θ`, `cos(θ) ≈ 1 - θ²/2` for `θ < 0.05`. But for learning,
> just use `sin`/`cos` and keep it correct.

---

## 4. Moving the Player

Translation is simpler. Move in the direction `dir` scaled by `move_speed * dt`:

```c
// Move forward
if (game->move_forward)
{
    double new_x = p->pos_x + p->dir_x * move_speed * game->dt;
    double new_y = p->pos_y + p->dir_y * move_speed * game->dt;
    // Collision check before applying
    if (map[int(p->pos_y)][int(new_x)] != '1')
        p->pos_x = new_x;
    if (map[int(new_y)][int(p->pos_x)] != '1')
        p->pos_y = new_y;
}
```

Notice: X and Y collisions are checked **separately**. This is what enables wall-sliding in `so_long`'s movement system too — if X collides but Y doesn't, you still move in Y.

---

## 5. Visualizing Camera Space

Before you write the raycaster, draw the vectors on your 2D debug canvas (Module 3). For each frame-step:

```
Canvas center = player position × cell_size
Dir vector endpoint = center + dir × 40 pixels
Plane left  endpoint = dir_endpoint - plane × 40
Plane right endpoint = dir_endpoint + plane × 40
```

This gives a visual T-shape showing exactly what "camera space" looks like before any raycasting.

---

## 6. Build Exercise

Add vector rendering to `examples/01_framebuffer` or a new `examples/02_vectors`:

1. Hard-code a player at grid position (3, 3), facing East.
2. Draw the player as a filled 8×8 white square.
3. Draw the direction vector as a red line 40px long.
4. Draw the camera plane endpoints as a blue line between them.
5. Wire up LEFT/RIGHT arrows to rotate the vectors in real time.

Confirm visually that the camera plane always stays perpendicular to the direction.

---

## Checkpoint

- [ ] Player facing direction is set correctly from `N/S/E/W`
- [ ] Left/right rotation updates both `dir` and `plane`
- [ ] Camera plane length stays constant (no drift) after 360° rotation
- [ ] Moving forward moves in the direction of `dir`

## Crisis Card

| Symptom | Fix |
|---------|-----|
| Player faces wrong way at spawn | Check your initial vector values per direction. Remember Y-down. |
| Camera plane drifts longer each rotation | You forgot to rotate `plane` together with `dir`. |
| Rotation is reversed | Flip the sign of the angle in `rotate_player`. |
| Player spawns inside a wall | Your map parser didn't reject the spawn position correctly. |

## Review Questions

1. Why is the camera plane perpendicular to the direction vector?
2. What happens to the FOV if you double the length of the camera plane?
3. Why do we check X and Y collisions separately instead of together?

## Journal Prompt

Draw the camera plane + direction vector on paper. Label `dir`, `plane`, the player position, and where the screen columns map to. Show it to your partner and explain it without looking at your notes.
