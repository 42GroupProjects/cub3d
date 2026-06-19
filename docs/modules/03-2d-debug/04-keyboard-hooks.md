# 04 — Asynchronous Keyboard Hooks

If you use `mlx_key_hook` to move the player, they will take one step, pause for 0.5s (due to macOS OS-level key repeating delays), and then start moving rapidly. This is unacceptable for an FPS engine.

We must record the boolean STATE of the keys, and check them every frame.

## 1. The Key State Array
Add a structure to your `t_game` struct:
```c
typedef struct s_keys {
    int w, a, s, d;
    int left, right;
} t_keys;
```

## 2. Hooking Press and Release
Hook the X11 `KeyPress` (2) and `KeyRelease` (3) events explicitly using `mlx_hook`.

```c
int key_press(int keycode, t_game *game)
{
    if (keycode == KEY_W) game->keys.w = 1;
    if (keycode == KEY_S) game->keys.s = 1;
    // ...
    return (0);
}

int key_release(int keycode, t_game *game)
{
    if (keycode == KEY_W) game->keys.w = 0;
    if (keycode == KEY_S) game->keys.s = 0;
    // ...
    return (0);
}
```

## 3. The Movement Logic
Run this every frame in your `game_loop` before you start rendering. Multiply by your Direction Vector so pressing "FORWARD" means moving in the direction you are facing.

```c
#define MOVE_SPEED 0.05
#define ROT_SPEED 0.03

void update_player(t_game *game)
{
    // Walking forward means ADDING the dir vector
    if (game->keys.w)
    {
        game->pos_x += game->dir_x * MOVE_SPEED;
        game->pos_y += game->dir_y * MOVE_SPEED;
    }
    // Walking backwards means SUBTRACTING the dir vector
    if (game->keys.s)
    {
        game->pos_x -= game->dir_x * MOVE_SPEED;
        game->pos_y -= game->dir_y * MOVE_SPEED;
    }
}
```

## 4. The Rotation Logic (Left/Right Arrows)
Use the 2D rotation matrix formulas on both `dir` and `plane` vectors when the user holds the Left or Right turn arrows. Remember that turning Left implies a negative angle in typical top-down graph math (or positive, depending on your Y-axis inversion).
