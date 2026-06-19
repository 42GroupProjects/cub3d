# 04-01 — DDA Algorithm Deep Dive

**Primary reference:** [Lode's Raycasting Tutorial Part 1](https://lodev.org/cgtutor/raycasting.html)

This is the complete DDA implementation walkthrough, line by line. Read Lode's tutorial first, then come back here to read the annotated real 42 student code.

---

## Step 0: Camera Space Setup

For each screen column `x` (0 to WIDTH−1), compute a camera-space coordinate:

```c
double camera_x = 2.0 * x / (double)WIDTH - 1.0;
//                 └── maps x: 0→-1 (left edge), W/2→0 (center), W-1→+1 (right edge)
```

Then compute the ray direction for this column:

```c
double ray_dir_x = dir_x + plane_x * camera_x;
double ray_dir_y = dir_y + plane_y * camera_x;
//
// dir   = player's facing direction (unit vector)
// plane = camera plane (perpendicular to dir, length controls FOV)
// The ray sweeps from (dir - plane) to (dir + plane) across all columns
```

**Visual understanding:** If you are facing East (`dir_x=1, dir_y=0`), the camera plane is `plane_x=0, plane_y=0.66`. The leftmost ray goes at `(1, -0.66)`, the center at `(1, 0)`, the rightmost at `(1, +0.66)`. This is the FOV sweep.

---

## Step 1: Delta Distances

```c
double delta_dist_x;
double delta_dist_y;

if (ray_dir_x == 0)
    delta_dist_x = 1e30;             // ray is horizontal — never crosses X grid lines
else
    delta_dist_x = fabs(1.0 / ray_dir_x);

if (ray_dir_y == 0)
    delta_dist_y = 1e30;
else
    delta_dist_y = fabs(1.0 / ray_dir_y);
```

**What is deltaDistX?** It is the distance the ray must travel (along the ray direction) to cross from one X-aligned grid line to the next. If the ray is nearly horizontal (small `|ray_dir_y|`), `delta_dist_y` is large — it takes a long ray-distance to cross a horizontal grid line.

**Reference — derfleck `raycast.c`:**
```c
/* derfleck/src/raycast.c — DDA setup (cleaned for readability) */

//raycasting using vectors — when direction is 0, delta is set to double max
void    raycast(t_map *map)
{
    int         x;
    t_player    *ptr;

    ptr = &map->play;
    x = 0;
    while (x < WIDTH)
    {
        ptr->camera_x = 2 * x / (double)WIDTH - 1;
        ptr->raydir[X] = ptr->look_dir[X] + (ptr->plane[X] * ptr->camera_x);
        ptr->raydir[Y] = ptr->look_dir[Y] + (ptr->plane[Y] * ptr->camera_x);

        // Guard against division by zero
        if (ptr->raydir[X] == 0)
            ptr->deltadist[X] = DBL_MAX;
        else
            ptr->deltadist[X] = fabs(1.0 / ptr->raydir[X]);
        if (ptr->raydir[Y] == 0)
            ptr->deltadist[Y] = DBL_MAX;
        else
            ptr->deltadist[Y] = fabs(1.0 / ptr->raydir[Y]);

        calc_steps(ptr);    // initialize sideDistX/Y and stepX/Y
        check_hit(ptr);     // DDA loop
        draw_wall_textured(map, x);
        x++;
    }
}
```

---

## Step 2: Side Distances and Step Direction

Side distance is the distance from the player to the **first** grid line in each direction:

```c
int     step_x;    // +1 if ray going right, -1 if going left
int     step_y;    // +1 if ray going down,  -1 if going up
double  side_dist_x;  // distance to first X grid line
double  side_dist_y;  // distance to first Y grid line

// Current grid cell:
int map_x = (int)pos_x;
int map_y = (int)pos_y;

// Fractional position within cell:
double frac_x = pos_x - (double)map_x;   // 0.0 = left edge, 1.0 = right edge
double frac_y = pos_y - (double)map_y;
```

**Reference — derfleck `calc_steps`:**
```c
/* derfleck/src/raycast.c — step direction and initial side distances */

static void    calc_steps(t_player *ptr)
{
    double    diff[2];

    // Fractional part of player position in current cell
    diff[X] = ptr->player[X] - (int)ptr->player[X];
    diff[Y] = ptr->player[Y] - (int)ptr->player[Y];

    if (ptr->raydir[X] < 0)
    {
        ptr->step[X] = -1;
        ptr->sidedist[X] = diff[X] * ptr->deltadist[X];
        // Distance to left X line = frac_x * deltaDistX
    }
    else
    {
        ptr->step[X] = 1;
        ptr->sidedist[X] = (1 - diff[X]) * ptr->deltadist[X];
        // Distance to right X line = (1 - frac_x) * deltaDistX
    }

    if (ptr->raydir[Y] < 0)
    {
        ptr->step[Y] = -1;
        ptr->sidedist[Y] = diff[Y] * ptr->deltadist[Y];
    }
    else
    {
        ptr->step[Y] = 1;
        ptr->sidedist[Y] = (1 - diff[Y]) * ptr->deltadist[Y];
    }
}
```

> **Understand `diff[X] * deltaDistX`:** If the player is 0.7 cells to the right of the left grid line and the ray goes left, the first X crossing is only 0.7 × deltaDistX away (not the full deltaDistX).

---

## Step 3: The DDA Loop

```c
/* derfleck/src/raycast.c — the DDA step loop */

static void    check_hit(t_player *ptr)
{
    int    hit;
    int    map[2];

    hit = 0;
    map[X] = (int)ptr->player[X];
    map[Y] = (int)ptr->player[Y];
    while (hit == 0)
    {
        // Which axis crossing comes next?
        if (ptr->sidedist[X] < ptr->sidedist[Y])
        {
            ptr->sidedist[X] += ptr->deltadist[X];   // advance to next X crossing
            map[X] += ptr->step[X];                   // step into next column
            ptr->side = 0;                             // X-axis wall hit
        }
        else
        {
            ptr->sidedist[Y] += ptr->deltadist[Y];   // advance to next Y crossing
            map[Y] += ptr->step[Y];
            ptr->side = 1;                             // Y-axis wall hit
        }
        // Did we hit a wall?
        if (map[Y] < 0 || map[X] < 0 ||
        ptr->map->map[map[Y]][map[X]] == 1)
            hit = 1;
    }
    calc_perpwalldist(ptr);
}
```

**Annotated line by line:**

| Line | Meaning |
|------|---------|
| `sidedist[X] < sidedist[Y]` | X grid crossing is closer than Y crossing — take the X step |
| `sidedist[X] += deltadist[X]` | Move the X side-distance forward by one cell-width |
| `map[X] += step[X]` | Enter the next grid column |
| `side = 0` | Record that we hit an X-aligned (vertical) wall |
| Check `map[map[Y]][map[X]] == 1` | Did we land in a wall cell? |

---

## ⚠️ CRISIS: The World Looks Round (Fisheye Effect)

**Symptom:** The walls curve outward at the sides of the screen. Moving forward makes it worse.

**Before (wrong — Euclidean distance):**
```c
// WRONG — this creates fisheye
double dist = sqrt((map_x - pos_x) * (map_x - pos_x) +
                   (map_y - pos_y) * (map_y - pos_y));
```

The Euclidean distance from the player to the wall hit point is longer for rays at the edges of the screen than for the center ray. This means edge walls appear shorter (farther) than they should, making the world appear to curve.

**After (correct — perpendicular distance):**
```c
/* derfleck/src/raycast.c — perpWallDist calculation */

static void    calc_perpwalldist(t_player *ptr)
{
    // Use the side distance BEFORE the last step, not after
    if (ptr->side)
        ptr->perpwalldist = ptr->sidedist[Y] - ptr->deltadist[Y];
    else
        ptr->perpwalldist = ptr->sidedist[X] - ptr->deltadist[X];
    if (ptr->perpwalldist <= 0)
        ptr->perpwalldist = DBL_MIN;  // prevent division by zero
}
```

**Why this works:** `sideDistX - deltaDistX` gives the distance along the ray to the last grid crossing, which is exactly where the wall was hit. Crucially, this is the **perpendicular** distance from the camera plane to the wall, not the Euclidean distance from the player. All columns use the same projection formula with this perpendicular distance, so the wall height is consistent regardless of the column angle — no fisheye.

---

## Step 4: Wall Projection

```c
// Wall height on screen (pixels)
int line_height = (int)(SCREEN_H / perp_wall_dist);

// Start and end pixels of the wall strip
int draw_start = SCREEN_H / 2 - line_height / 2;
if (draw_start < 0)
    draw_start = 0;

int draw_end = SCREEN_H / 2 + line_height / 2;
if (draw_end >= SCREEN_H)
    draw_end = SCREEN_H - 1;
```

**Clamp** `draw_start` and `draw_end` to the screen boundaries — very close walls produce `line_height > SCREEN_H`, which would crash an unclamped loop.

Draw the vertical strip from `draw_start` to `draw_end` in a flat color (X-side slightly darker):

```c
unsigned int color;
if (side == 0)
    color = 0x888888;   // X-wall (vertical surface) — brighter
else
    color = 0x444444;   // Y-wall (horizontal surface) — darker for depth cue
while (draw_start <= draw_end)
    put_pixel(&img, x, draw_start++, color);
```
