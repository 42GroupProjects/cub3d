# Module 07 — Official Bonus

**Branch:** `bonus` | **Days:** 13–15

> **Prerequisite:** Module 06 mandatory freeze checklist complete on `main`. Do not start here until mandatory is perfect.

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | Overview + checkpoint |
| [01-minimap.md](./01-minimap.md) | Minimap rendering with real mathias-mrsn code |
| [02-mouse-rotation.md](./02-mouse-rotation.md) | Mouse look with re-centering trick |
| [03-doors.md](./03-doors.md) | Door state machine with real mathias-mrsn code |
| [04-sliding-collision.md](./04-sliding-collision.md) | Full sliding wall collision |
| [05-animated-sprites.md](./05-animated-sprites.md) | Sprite projection + Z-buffer |

**Code references:**
- `code_snippets/bonus/mathias_minimap.c` — full minimap draw loop with player and border
- `code_snippets/bonus/mathias_door.c` — door state machine with animated open/close
- `code_snippets/bonus/mathias_mouse_handler.c` — mouse position tracking + re-centering
- `code_snippets/bonus/mathias_key_handler.c` — key press/release flags + mouse_move

---

## Build Order (Recommended)

1. **Minimap** — highest visual payoff, easiest to debug
2. **Mouse look** — one hook + re-centering, quick win
3. **Sliding collision** — upgrade the mandatory wall-stop
4. **Doors** — new map character + state machine
5. **Animated sprites** — hardest, needs Z-buffer

---

## The Minimap Logic

**Reference — mathias-mrsn `minimap.c`:**

```c
/* mathias-mrsn/srcs_bonus/minimap/minimap.c — minimap draw with player centered */

/*
 * The minimap is player-centered. Each minimap pixel (pxl_x, pxl_y)
 * corresponds to a world position calculated relative to the player.
 * DIST_PER_PIXEL controls the map zoom level.
 * CENTER_X and CENTER_Y are the pixel center of the minimap.
 */
void    draw_elem(t_cub *s, int pxl_x, int pxl_y)
{
    double    x;
    double    y;

    // World coordinates for this minimap pixel
    x = s->player->p_x - (double)(DIST_PER_PIXEL * (CENTER_X - pxl_x));
    y = s->player->p_y - (double)(DIST_PER_PIXEL * (CENTER_Y - pxl_y));

    if (x > 0.0 && y > 0.0 && x < s->map_width && y < s->map_height
        && s->map[(int)y][(int)x] == '1')
        __put_pixel_on_img(&s->img,
            (pxl_x + PADDING_X + BORDER),
            (pxl_y + PADDING_Y + BORDER),
            WALL_COLOR);
    else if (/* similarly check '0', 'D', '2', etc. */ false)
        ; // other tile types
    else
        __put_pixel_on_img(&s->img,
            (pxl_x + PADDING_X + BORDER),
            (pxl_y + PADDING_Y + BORDER),
            BACKGROUND_COLOR);
}

void    map2d(t_cub *s)
{
    int    pxl_x;
    int    pxl_y;

    if (s->win_x < 300 || s->win_y < 300)
        return ;                // skip if window is too small
    draw_border(s);
    pxl_x = 0;
    while (pxl_x < MINIMAP_W)
    {
        pxl_y = 0;
        while (pxl_y < MINIMAP_H)
        {
            draw_elem(s, pxl_x, pxl_y);
            pxl_y++;
        }
        pxl_x++;
    }
    draw_player(s);             // draw player dot at center
}
```

> **Key formula:** `x = player_x - DIST_PER_PIXEL * (CENTER_X - pxl_x)`. When `pxl_x = CENTER_X`, `x = player_x`. Pixels to the left of center map to world positions to the left of the player; pixels to the right map right. `DIST_PER_PIXEL` controls zoom: smaller = zoomed in.

---

## Mouse Look — The Re-centering Trick

**Reference — mathias-mrsn `key_handler.c` macOS version:**

```c
/* mathias-mrsn/srcs_bonus/moves/key_handler.c — mouse look */

void    mouse_move(t_cub *s)
{
    int    x;
    int    y;

    mlx_mouse_get_pos(s->win, &x, &y);

    // RAD_PER_PIXEL: how many radians of rotation per pixel of mouse movement
    s->player->angle = trigo_cercle(s->player->angle
        + (s->moves->last_x_mouse - x) * RAD_PER_PIXEL);

    // Re-center the mouse each frame to prevent hitting screen edge
    s->moves->last_x_mouse = s->win_x / 2;
    mlx_mouse_move(s->win, s->win_x / 2, s->win_y / 2);
}
```

> **The re-centering trick:** Each frame, after reading the mouse delta, we teleport the cursor back to the center of the window. Next frame, the delta is relative to center again. This prevents the cursor from hitting the screen edge and stopping input. The first frame after re-centering must be skipped (delta from last_x to center would be huge); track this with an `initialized` flag.

---

## Door State Machine

**Reference — mathias-mrsn `door.c`:**

```c
/* mathias-mrsn/srcs_bonus/engine/door.c — door state machine */

/*
 * Door states:
 *   0 = CLOSED
 *   1 = OPENING  (animation in progress)
 *   2 = OPEN     (fully open, player can pass)
 *   3 = CLOSING  (animation in progress)
 */
static void    open_door(t_door *door)
{
    if (door->is_open == 0)
    {
        door->is_open = 1;          // start opening
        door->start = clock() / _CLOCK_TIME;
    }
    else if (door->is_open == 1)
    {
        // Animate: increase opening_x (0.0 = closed, 1.0 = fully open)
        door->opening_x += ((clock() / _CLOCK_TIME) -
            door->start) * DOOR_OPENING_SPEED;
        door->start = clock() / _CLOCK_TIME;
        if (door->opening_x >= 1.0)
        {
            door->is_open = 2;      // fully open
            door->opening_x = 1.0;
        }
    }
    else if (door->is_open == 3)    // was closing — reverse direction
    {
        door->start = clock() / _CLOCK_TIME;
        door->is_open = 1;
    }
}

/*
 * update_doors: called each frame.
 * If player is close to a door → open it.
 * If player is far → close it.
 */
void    update_doors(t_cub *s)
{
    uint32_t    idx;

    idx = 0;
    while (idx < s->nbr_door)
    {
        if (__pythagore(s->player->p_x, s->player->p_y,
                s->doors[idx].x, s->doors[idx].y) < OPENING_DOOR_DIST)
            open_door(&s->doors[idx]);
        else
            close_door(&s->doors[idx]);
        idx++;
    }
}
```

> **The `opening_x` trick:** Rather than an on/off toggle, the door has a float `opening_x` from 0.0 (closed) to 1.0 (open). The raycaster uses this value to determine if the ray should pass through the door or hit it: if `get_decimal(ray_y) > opening_x`, the door is still blocking at that point.

---

## Exit Checkpoint

- [ ] Minimap renders in screen corner — player dot visible at center
- [ ] Mouse rotation is smooth — no first-frame jump, no drift toward edges
- [ ] Player slides along walls diagonally — does not get stuck
- [ ] Door toggles between open/close on proximity
- [ ] Animated sprites sort correctly by depth (closer ones render over farther ones)
