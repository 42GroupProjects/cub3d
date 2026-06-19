# 05-01 — Texture Mapping

---

## Loading XPM Textures

```c
t_image    tex;
tex.img    = mlx_xpm_file_to_image(game->mlx, path, &tex.width, &tex.height);
if (tex.img == NULL)
{
    // clean up textures loaded so far, then exit
    ft_putendl_fd("Error\nFailed to load texture", 2);
    cleanup_and_exit(game);
}
tex.pixels = mlx_get_data_addr(tex.img, &tex.bits_per_pixel,
                               &tex.line_length, &tex.endian);
```

**Reference — derfleck `textures.c` load with rollback:**

```c
/* derfleck/src/textures.c — texture loading with rollback on failure */

void    load_textures(t_map *map)
{
    int    i;
    int    tex;

    i = 0;
    tex = TEX;    // TEX = texture size constant (e.g. 64)
    while (i < 4)
    {
        map->tex[i].img = mlx_xpm_file_to_image(map->mlx.mlx, map->path[i],
            &tex, &tex);
        if (map->tex[i].img == NULL)
        {
            while (--i >= 0)
                mlx_destroy_image(map->mlx.mlx, map->tex[i].img);
            return (perror("Error loading textures\n"));
        }
        map->tex[i].addr = mlx_get_data_addr(map->tex[i].img,
            &map->tex[i].bpp, &map->tex[i].line_len, &map->tex[i].endian);
        if (map->tex[i].addr == NULL)
        {
            while (--i >= 0)
                mlx_destroy_image(map->mlx.mlx, map->tex[i].img);
            return (perror("Error getting texture address\n"));
        }
        i++;
    }
}
```

> **Notice the rollback:** if texture `i` fails, the `while (--i >= 0)` loop destroys all previously loaded textures before returning. This prevents leaks on partial load failure.

---

## Which Texture to Use — Wall Selection

**Reference — derfleck `draw_wall_textured`:**

```c
/* derfleck/src/textures.c — texture selection by wall side + ray direction */

void    draw_wall_textured(t_map *map, int x)
{
    // ...
    while (play->screen_pos[Y] <= end)
    {
        play->tex_i[Y] = (int)play->tex_pos & (TEX - 1);  // wrap tex_y to texture height
        play->tex_pos += play->tex_step;

        if (!play->side && play->raydir[X] < 0)            // X wall, ray going left = WEST
            draw_texture(map, play->screen_pos, WEST);
        else if (!play->side && play->raydir[X] > 0)       // X wall, ray going right = EAST
            draw_texture(map, play->screen_pos, EAST);
        else if (play->side && play->raydir[Y] < 0)        // Y wall, ray going up = NORTH
            draw_texture(map, play->screen_pos, NORTH);
        else if (play->side && play->raydir[Y] > 0)        // Y wall, ray going down = SOUTH
            draw_texture(map, play->screen_pos, SOUTH);
        play->screen_pos[Y]++;
    }
}
```

**Memory aid for texture selection:**

| `side` | `raydir[X or Y] sign` | Texture |
|--------|----------------------|---------|
| 0 (X wall) | `raydir[X] < 0` (ray going left) | WEST |
| 0 (X wall) | `raydir[X] > 0` (ray going right) | EAST |
| 1 (Y wall) | `raydir[Y] < 0` (ray going up = North) | NORTH |
| 1 (Y wall) | `raydir[Y] > 0` (ray going down = South) | SOUTH |

---

## Wall Hit X Coordinate (`wall_x`)

`wall_x` is the exact fractional position where the ray hit the wall — 0.0 at the left edge of the cell, 1.0 at the right edge. This gives us the texture column to sample.

```c
/* derfleck/src/textures.c — wall_x calculation */

static void    calc_wall_height(t_player *play, int x)
{
    play->wall_height = (int)(HEIGHT / play->perpwalldist);
    play->screen_pos[X] = x;
    play->screen_pos[Y] = -play->wall_height / 2 + HEIGHT / 2;   // center on screen
    if (play->screen_pos[Y] < 0)
        play->screen_pos[Y] = 0;

    // wall_x: exact hit coordinate on the wall face
    if (play->side)    // Y wall hit: use player X position + ray travel
        play->wall_x = play->player[X] + play->perpwalldist * play->raydir[X];
    else               // X wall hit: use player Y position + ray travel
        play->wall_x = play->player[Y] + play->perpwalldist * play->raydir[Y];
    play->wall_x -= floor(play->wall_x);   // keep only 0.0–1.0 fractional part

    // texture X column from wall_x
    play->tex_i[X] = (int)(play->wall_x * (double)TEX);

    // Mirror correction — prevents texture appearing flipped on some wall faces
    if ((play->side == 0 && play->raydir[X] < 0) ||
    (play->side == 1 && play->raydir[Y] > 0))
        play->tex_i[X] = TEX - play->tex_i[X] - 1;

    // Texture Y stepping
    play->tex_step = 1.0 * ((double)TEX / (double)play->wall_height);
    play->tex_pos = (play->screen_pos[Y] - HEIGHT / 2 +
        play->wall_height / 2) * play->tex_step;
}
```

---

## ⚠️ CRISIS: The Stretching Seizure (Texture Y Calculation)

**Symptom:** Textures look stretched or warped at close range. The top and bottom of the texture bleed into each other.

**Root cause:** `tex_y` must be computed as a **scaled integer** sampling of the texture, not a raw pixel offset. If you use a float that overflows the texture height, you index into unrelated memory.

**The fix — `tex_y` with bitwise wrap (power-of-2 textures only):**

```c
// Inside the per-pixel loop:
int tex_y = (int)tex_pos & (tex_height - 1);
    //             ^                  ^
    //             |                  |  bitwise modulo: works ONLY if tex_height = 64, 128, 256...
    //             float converted to int

tex_pos += tex_step;   // advance fractional Y position
```

**Why bitwise `&`?** `& (N-1)` is faster than `% N` and is safe when `N` is a power of 2 (64, 128, 256). If your texture is 64px tall: `(int)tex_pos & 63` wraps 0–63, 64–127, etc. This is exactly what derfleck does: `play->tex_i[Y] = (int)play->tex_pos & (TEX - 1)`.

> **Historical note from the Game Engine Black Book (Wolfenstein 3D):**
> Wolfenstein 3D was limited to integer arithmetic for all texture lookups. John Carmack used fixed-point math — storing numbers as 16.16 integers (16 bits integer, 16 bits fractional) to simulate floats without the CPU penalty. In cub3D you use actual floats, but the underlying sampling discipline — "compute tex_y as a scaled integer index" — is exactly the same principle. The `& (TEX-1)` trick is a direct descendant of this fixed-point approach.

---

## Floor and Ceiling Fill

For mandatory, floor and ceiling are solid colors — no texture required.

```c
// Fill ceiling: rows 0 to draw_start - 1
for (int y = 0; y < draw_start; y++)
    put_pixel(&game->img, x, y, game->color_ceil);

// Fill floor: rows draw_end + 1 to SCREEN_H - 1
for (int y = draw_end + 1; y < SCREEN_H; y++)
    put_pixel(&game->img, x, y, game->color_floor);
```

Pre-compute `color_ceil` and `color_floor` once from parsed RGB values:

```c
game->color_floor = (floor_r << 16) | (floor_g << 8) | floor_b;
game->color_ceil  = (ceil_r  << 16) | (ceil_g  << 8) | ceil_b;
```

---

## ⚠️ Common Texture Logic Mistakes

**Mistake 1: Using `line_length` as `width * bpp` (❌)**

```c
/* WRONG — assumes line_length == width * 4 */
int offset = y * (tex.width * 4) + x * 4;
unsigned int color = *(unsigned int *)(tex.pixels + offset);
```

The OS may pad each row to a multiple of 4 or 8 bytes for memory alignment. If `tex.width = 50`, the actual `line_length` might be `200` (50 * 4, no padding) or `208` (padded). Using `50 * 4 = 200` when the real stride is `208` causes every row below row 0 to read 8 bytes too early — textures appear diagonally compressed and Valgrind reports out-of-bounds reads.

**Fix:** Always use the `line_length` value returned by `mlx_get_data_addr`:
```c
int offset = y * tex.line_length + x * (tex.bits_per_pixel / 8);
unsigned int color = *(unsigned int *)(tex.pixels + offset);
```

---

**Mistake 2: Loading textures before `mlx_init` (❌)**

```c
/* WRONG — calling mlx_xpm_file_to_image before mlx_init */
int main(void)
{
    t_game game;
    game.tex[0].img = mlx_xpm_file_to_image(NULL, "wall.xpm", ...); // NULL mlx!
    game.mlx = mlx_init();
}
```

`mlx_xpm_file_to_image` requires a valid MLX context to allocate the image. Passing `NULL` as the first argument causes a segfault deep inside MLX's internal XPM decoder with no useful error message.

**Fix:** Always `mlx_init()` first, then load textures:
```c
game.mlx = mlx_init();
if (!game.mlx)
    return (ft_error("mlx_init failed"));
load_all_textures(&game); // Load all XPM files after context exists
```

---

**Mistake 3: Selecting wrong texture face — using ray direction instead of `side` (❌)**

```c
/* WRONG — only checks rayDirX, ignores which axis was actually hit */
if (ray_dir_x < 0)
    tex = game->tex[WEST];
else
    tex = game->tex[EAST];
// NORTH and SOUTH walls are NEVER used!
```

North and South walls are hit when `side == 1` (the ray crossed a horizontal Y grid line), not when `rayDirX` is negative. This code always uses WEST or EAST textures regardless of the actual wall orientation.

**Fix:** Branch on `side` first, then direction:
```c
if (side == 0 && ray_dir_x < 0) tex = game->tex[WEST];
if (side == 0 && ray_dir_x > 0) tex = game->tex[EAST];
if (side == 1 && ray_dir_y < 0) tex = game->tex[NORTH];
if (side == 1 && ray_dir_y > 0) tex = game->tex[SOUTH];
```
