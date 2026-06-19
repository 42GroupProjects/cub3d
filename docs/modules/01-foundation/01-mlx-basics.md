# 01 — MLX Basics: Window, Hooks, and the Game Loop

## Outcome
You can open a window, register keyboard hooks properly, and run a controlled game loop that executes at a fixed timestep.

---

## 1. Core MLX Lifecycle

MiniLibX has a rigid lifecycle you must follow in order. Mess up the order and you get segfaults.

```
mlx_init()
  → mlx_new_window()
    → mlx_hook() × N        ← register keyboard/mouse/close events
    → mlx_loop_hook()       ← register your per-frame update function
      → mlx_loop()          ← block here forever, driving your hooks
```

Every one of those returns a pointer or an int. **Check every return value.** `mlx_init()` can return `NULL`. So can `mlx_new_window()`. If either is `NULL`, you must print `Error\n` and exit cleanly.

### ❌ Common Ordering Mistakes

**Mistake 1: Calling `mlx_loop` before hooks**

```c
/* WRONG — loop starts before hooks are registered */
int main(void)
{
    void    *mlx;
    void    *win;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 800, 600, "cub3d");
    mlx_loop(mlx);              // Starts the loop immediately!
    mlx_hook(win, 2, 2, ...);  // This line is NEVER reached.
    return (0);
}
```
`mlx_loop` is a blocking infinite loop. Any code below it never executes.

**Mistake 2: Not checking return values**

```c
/* WRONG — NULL pointer crash waiting to happen */
int main(void)
{
    void    *mlx;
    void    *win;

    mlx = mlx_init();              // Returns NULL on Apple Silicon without Rosetta
    win = mlx_new_window(mlx, ...); // Segfault: dereferencing NULL mlx
    mlx_loop(mlx);
}
```
On Apple Silicon without `arch -x86_64`, `mlx_init()` silently returns `NULL`. The next call dereferences it and crashes immediately.

**Mistake 3: Forgetting the DestroyNotify hook for the red X button**

```c
/* WRONG — red X button does nothing */
mlx_hook(win, 2, 2, on_keypress, game);
mlx_loop_hook(mlx, game_loop, game);
mlx_loop(mlx);
// User clicks the red X → nothing happens. Program freezes.
```

Without `mlx_hook(win, 17, 0, on_close, game)`, the window close button is unregistered. The evaluator will fail your submission for this on the spot.

### ✅ The Correct Pattern

```c
int main(void)
{
    t_game  game;

    ft_memset(&game, 0, sizeof(t_game)); // Zero all pointers first
    game.mlx = mlx_init();
    if (!game.mlx)
        return (ft_error("mlx_init failed", 1));
    game.win = mlx_new_window(game.mlx, WIN_W, WIN_H, "cub3d");
    if (!game.win)
        return (ft_error("window creation failed", 1));
    // Register ALL hooks BEFORE loop
    mlx_hook(game.win, 2,  2, on_keydown, &game);
    mlx_hook(game.win, 3,  3, on_keyup,   &game);
    mlx_hook(game.win, 17, 0, on_close,   &game); // Red X button!
    mlx_loop_hook(game.mlx, game_loop, &game);
    mlx_loop(game.mlx); // Block here forever
    return (0);
}
```



## 2. The Hook System

There are two kinds of hooks:

| Hook | Purpose |
|------|---------|
| `mlx_hook(win, event, mask, fn, param)` | App-level event (key press, close button) |
| `mlx_loop_hook(mlx, fn, param)` | Called every iteration of the inner event loop |

**Event numbers that matter for cub3d:**

| Event | Number | Mask |
|-------|--------|------|
| Key press | 2 | 1<<0 (or just `2`) |
| Key release | 3 | 1<<1 (or just `3`) |
| Window close (X button) | 17 | 0 |

```c
mlx_hook(game->window, 2,  2, on_key_down, game);
mlx_hook(game->window, 3,  3, on_key_up,   game);
mlx_hook(game->window, 17, 0, on_destroy,  game);
mlx_loop_hook(game->mlx, game_loop, game);
```

> [!IMPORTANT]
> On macOS, call `mlx_do_key_autorepeatoff(game->mlx)` after `mlx_loop_hook` to prevent the OS from sending repeated key-down events when a key is held. Without this, movement is unpredictable.

**Key codes differ by platform:**

```c
// macOS
#define KEY_W  13
#define KEY_A   0
#define KEY_S   1
#define KEY_D   2
#define KEY_ESC 53
#define KEY_LEFT  123
#define KEY_RIGHT 124
```

---

## 3. The Fixed Timestep Game Loop

Your `mlx_loop_hook` callback is called as fast as the system allows — which can be 400+ times per second on a fast machine. This is a problem: physics and animation that depend on time will run at different speeds on different hardware.

**Solution:** Fix the target time per frame, then *spin-wait* until the frame is complete.

```c
// in game_init: set a fixed target of 30 fps
game->dt = 1.0f / 30;

// in your loop hook:
int game_loop(t_game *game)
{
    clock_t start = clock();

    // --- all simulation and drawing happens here ---
    update_game(game);
    draw_game(game);
    // -----------------------------------------------

    // Spin-wait until the target frame time has elapsed
    clock_t elapsed = clock() - start;
    while ((double)elapsed / CLOCKS_PER_SEC < game->dt)
        elapsed = clock() - start;

    // Only now push the image to the screen
    mlx_put_image_to_window(game->mlx, game->window,
        game->window_image.img, 0, 0);
    return (0);
}
```

> [!NOTE]
> `game->dt` is reused everywhere else to make physics frame-rate independent.
> Each velocity update multiplies by `dt` so a 30fps simulation moves the same
> distance as a 60fps simulation in the same real-world time.

---

## 4. Key State vs Key Event

There are two philosophies for input:

1. **Event-driven**: `on_key_down` fires once when pressed. You track direction in a flag.
2. **State poll** (not directly available in MLX): requires you to maintain your own `int keys[256]` array.

For Cub3D, the event-driven approach works well. Store intent in your player struct:

```c
// on key_down
if (keycode == KEY_W) game->move_forward = 1;
if (keycode == KEY_A) game->move_left    = 1;

// on key_up
if (keycode == KEY_W) game->move_forward = 0;
if (keycode == KEY_A) game->move_left    = 0;
```

Then in `game_loop`, read these flags and compute physics by `dt`.

---

## 5. Build Exercise

Create `examples/00_window/main.c` from scratch. It must:

1. `mlx_init()` → check NULL
2. `mlx_new_window()` → check NULL
3. Register ESC-to-exit hook
4. Register window-close (event 17) hook
5. Register a `game_loop` that prints the frame number to stdout every 30 frames
6. `mlx_loop()` blocking call

Do **not** use any image buffer yet. Just confirm the window opens, you can see it, and ESC closes it cleanly.

---

## Checkpoint

- [ ] Window opens with a title
- [ ] ESC exits without crashing
- [ ] Window X button exits without crashing
- [ ] Frame counter prints to stdout at steady 30fps cadence
- [ ] `dt = 1/30` spin-wait is in place

## Crisis Card

| Symptom | Fix |
|---------|-----|
| Nothing appears | Did you call `mlx_loop()`? It is blocking. |
| Window opens then immediately closes | `mlx_loop()` is missing or placed before hooks. |
| Key repeat fires rapidly when key held | Add `mlx_do_key_autorepeatoff()`. |
| Segfault on exit | Your close hook isn't calling `mlx_destroy_window` before `exit`. |

## Review Questions

1. Why does `mlx_loop()` never return normally?
2. What is the purpose of the `mask` parameter in `mlx_hook`?
3. If `game->dt = 1.0 / 30`, what does multiplying velocity by `dt` guarantee?

## Journal Prompt

Open your `daily-template.md` for today. Write:
- The moment you got a clean window open with working ESC
- One thing about the hook system that surprised you
- One question you cannot yet answer
