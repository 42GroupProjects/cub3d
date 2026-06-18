# 01 — Structural Norm Compliance

Passing the 42 Norm in the Cub3D engine is often harder than the rendering math. The 25-line limit and 4-parameter limit per function will force you to architect your data securely.

## 1. The Super-Struct
You cannot pass `pos_x`, `pos_y`, `dir_x`, `dir_y`, `plane_x`, `plane_y` into your DDA function. That is 6 parameters. 

**Solution:** Group your data tightly into nested structs.

```c
typedef struct s_player {
    double x;
    double y;
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
} t_player;

typedef struct s_game {
    t_player    player;
    t_img       img;
    char        **map;
    // ...
} t_game;
```
Now, you only ever pass `t_game *game` into your functions!

## 2. Breaking Down the Raycasting Loop
A standard DDA loop is 100+ lines. It must be chopped up into logical chunks.

**Example Refactoring:**
```c
// 1. Mother function
void cast_all_rays(t_game *game)
{
    int x = 0;
    while (x < SCREEN_W)
    {
        cast_single_ray(game, x);
        x++;
    }
}

// 2. The core raycaster
void cast_single_ray(t_game *game, int x)
{
    t_ray ray;
    
    init_ray_vectors(game, &ray, x);    // 25 lines
    calc_step_and_sideDist(game, &ray); // 20 lines
    perform_dda(game, &ray);            // 15 lines
    render_wall_stripe(game, &ray, x);  // 25 lines
}
```

## 3. The Ternary Trick
If you are struggling with lines, the Norm allows ternary operators.
`int dir = (x > 0) ? 1 : -1;` saves you 4 lines of `if/else` brackets. Use them aggressively for `step_x`/`step_y` calculations.
