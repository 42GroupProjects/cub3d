# 05-02 — Movement and Collision

---

## Frame-Rate-Independent Speed (the `dt` pattern)

Movement speed and rotation speed must scale with time elapsed per frame, not with raw FPS. This is the `dt` pattern from your `so_long` project.

**Reference — derfleck `move.c` — `calc_speed` with FPS cap:**

```c
/* derfleck/src/move.c — frame timing and FPS-independent speed */

void    calc_speed(t_player *play)
{
    double    diff;              // time elapsed since last frame (seconds)
    double    target_frame_time;
    double    time_elapsed;
    clock_t   start_time;

    target_frame_time = 1.0 / TARGET_FPS;    // e.g. 1/30 = 0.0333...
    if (play->prev_time == 0)
        play->prev_time = clock();
    diff = (double)(clock() - play->prev_time) / CLOCKS_PER_SEC;
    play->prev_time = clock();

    // FPS cap: spin-wait until target frame time is reached
    if (TARGET_FPS && diff < target_frame_time)
    {
        start_time = clock();
        time_elapsed = target_frame_time - diff;
        while ((double)(clock() - start_time) / CLOCKS_PER_SEC < time_elapsed)
            ;
        diff = target_frame_time;
    }

    play->map->walk_speed = diff * 5;    // scale speed by dt
    play->map->rot_speed  = diff * 300;  // scale rotation by dt
    if (PRINT_FPS)
        printf("%d fps\r", (int)(1 / diff));
}
```

> **The key insight:** `walk_speed = diff * 5` where `diff` is seconds-per-frame. At 30fps, `diff ≈ 0.033`, so `walk_speed ≈ 0.165` units/frame. At 60fps, `diff ≈ 0.016`, so `walk_speed ≈ 0.08` per frame — but twice as many frames — same total movement per second. This is frame-rate-independent movement.

---

## Walk (W/S) and Strafe (A/D) Logic

### Independent Axis Collision (AABB)
When a player walks diagonally into a wall, they should not get completely stuck. They should slide along the wall. To achieve this, you must test the `X` movement and the `Y` movement completely independently.

```mermaid
flowchart TD
    Start[Calculate Desired Speed<br/>speedX = dirX * walkSpeed<br/>speedY = dirY * walkSpeed] --> CheckX
    
    CheckX{map[posY][posX + speedX]<br/>== 0 ?}
    CheckX -- Yes --> MoveX[posX += speedX]
    CheckX -- No --> BlockX[Slide: Do not move X]
    
    MoveX --> CheckY
    BlockX --> CheckY
    
    CheckY{map[posY + speedY][posX]<br/>== 0 ?}
    CheckY -- Yes --> MoveY[posY += speedY]
    CheckY -- No --> BlockY[Slide: Do not move Y]
    
    MoveY --> End[Frame Complete]
    BlockY --> End
```

**Reference — derfleck `walk` and `strafe`:**

```c
/* derfleck/src/move.c — walk forward/backward */

void    walk(t_map *map, int dir)
{
    t_player    *play;
    double       speed[2];
    int          next[2];

    play = &map->play;
    speed[X] = map->walk_speed * play->look_dir[X];
    speed[Y] = map->walk_speed * play->look_dir[Y];
    if (dir == SOUTH)   // backward = invert speed
    {
        speed[X] = -speed[X];
        speed[Y] = -speed[Y];
    }
    // Compute integer grid cell of next position
    next[X] = (int)(play->player[X] + speed[X]);
    next[Y] = (int)(play->player[Y] + speed[Y]);

    // Only move if next cell is not a wall
    if (next[X] < map->max[X] && next[Y] < map->max[Y] &&
    map->map[next[Y]][next[X]] == 0)
    {
        if ((next[X] != (int)play->player[X] ||
        next[Y] != (int)play->player[Y]) && !check_corner(map, next, speed))
            return ;    // corner slip prevention
        play->player[X] += speed[X];
        play->player[Y] += speed[Y];
    }
}

/* derfleck/src/move.c — strafe left/right using perpendicular direction */

void    strafe(t_map *map, int dir)
{
    t_player    *play;
    double       speed[2];
    double       look_side[2];
    double       ang;

    play = &map->play;
    ang = deg_to_rad(90);
    if (dir == WEST)
        ang = deg_to_rad(-90);

    // Rotate the look direction 90° to get the strafe direction
    look_side[X] = play->look_dir[X] * cos(ang) - play->look_dir[Y] * sin(ang);
    look_side[Y] = play->look_dir[X] * sin(ang) + play->look_dir[Y] * cos(ang);
    speed[X] = map->walk_speed * look_side[X];
    speed[Y] = map->walk_speed * look_side[Y];

    // ... same collision check pattern as walk
}
```

> **Strafe direction:** Rotating the direction vector 90° gives the strafe axis. `cos(90°) = 0, sin(90°) = 1` so `look_side = (-dir_y, dir_x)`. This is the perpendicular direction.

---

## Rotation

**Reference — derfleck `rotate` with matrix multiplication:**

```c
/* derfleck/src/move.c — rotation matrix applied to look_dir + camera plane */

void    rotate(t_map *map, double ang)
{
    t_mat    rot_mat;

    // 2D rotation matrix: [[cos, -sin], [sin, cos]]
    rot_mat = (t_mat){cos(ang), -sin(ang), sin(ang), cos(ang)};
    mat_mul(&map->play.look_dir[0], rot_mat);    // rotate direction vector
    mat_mul(&map->play.plane[0], rot_mat);       // rotate camera plane (same angle!)
}
```

**Why rotate both?** `dir` and `plane` are always perpendicular and `plane` length encodes FOV. Rotating only `dir` rotates the view center but not the FOV extent — walls appear to rotate correctly but everything gets distorted. Both vectors must rotate by the same angle.

**Rotation formula (inline version without matrix helper):**

```c
void    rotate_player(t_game *g, double angle)
{
    double    old_dir_x;
    double    old_plane_x;

    old_dir_x   = g->dir_x;    // MUST save before overwriting
    old_plane_x = g->plane_x;

    g->dir_x   = old_dir_x   * cos(angle) - g->dir_y   * sin(angle);
    g->dir_y   = old_dir_x   * sin(angle) + g->dir_y   * cos(angle);
    g->plane_x = old_plane_x * cos(angle) - g->plane_y * sin(angle);
    g->plane_y = old_plane_x * sin(angle) + g->plane_y * cos(angle);
}
```

> **Critical:** Save `old_dir_x` and `old_plane_x` before the computation. The formula needs the original values for both components. If you overwrite `dir_x` first, the second formula reads the wrong (already rotated) value.

---

## Corner Checking (Bonus Movement Quality)

The `check_corner` function prevents the player from slipping through wall corners when moving diagonally. Only include this after your basic movement works.

```c
/* derfleck/src/move.c — prevent corner slippage */

static int    check_corner(t_map *map, int *next, double *speed)
{
    // When moving diagonally, check that the adjacent cells in each
    // direction are not also walls — if they are, the player would
    // clip through the corner.
    if (speed[X] > 0 && speed[Y] < 0)    // moving right+up
    {
        if (next[X] - 1 >= 0 && map->map[next[Y]][next[X] - 1] == 1 &&
        next[Y] + 1 < map->max[Y] && map->map[next[Y] + 1][next[X]] == 1)
            return (0);    // both adjacent cells are walls — block movement
    }
    // ... similar checks for other diagonal directions
    return (1);    // movement is safe
}
```

> **For mandatory submissions:** a simple X/Y axis split (try moving X, try moving Y independently) is sufficient and correct without the corner check. Add `check_corner` only if you observe players slipping through corners.
