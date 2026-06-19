# MacOS vs Ubuntu: Cross-Platform Guide

Cub3D is evaluated on 42 cluster machines. 
Some campuses use iMacs with Apple Silicon (M1/M2) or Intel (AppKit). Others run Ubuntu machines (X11). 
Code written perfectly on macOS will **fail to compile** or **segfault on closing** on an Ubuntu machine.

You must build conditional macros (`#ifdef`) into your code to handle these operating systems dynamically.

## 1. Keycode Translation

macOS virtual keycodes are totally different from X11 `keysyms`. 

> **macOS W key**: `13`
> **Ubuntu W key**: `'w'` or `119` (0x0077)

### The Solution: The `includes/keys.h` trick
Create a header file specifically for key bindings using standard OS detection macros.

```c
#ifndef KEYS_H
# define KEYS_H

# ifdef __linux__
#  include <X11/keysym.h>
#  define KEY_ESC   XK_Escape
#  define KEY_W     XK_w
#  define KEY_A     XK_a
#  define KEY_S     XK_s
#  define KEY_D     XK_d
#  define KEY_LEFT  XK_Left
#  define KEY_RIGHT XK_Right
# else
#  define KEY_ESC   53
#  define KEY_W     13
#  define KEY_A     0
#  define KEY_S     1
#  define KEY_D     2
#  define KEY_LEFT  123
#  define KEY_RIGHT 124
# endif

#endif
```

## 2. Destroying the Display (Memory Leaks)

The `leaks` command on macOS doesn't care if you leave the base `mlx` pointer allocated on exit. Valgrind on Ubuntu *strictly* requires you to free everything, and the X11 connection must be explicitly severed.

To do this, Ubuntu MLX has a function called `mlx_destroy_display(void *mlx_ptr)`.
**This function does not exist in the macOS MLX library!** If you try to compile `mlx_destroy_display` on a Mac, you get a linker error.

### The Solution
Inside your `clean_exit` function, conditionally compile the destruction sequence.

```c
void clean_exit(t_game *game)
{
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    
    // Linux-only Display freeing
    #ifdef __linux__
    if (game->mlx)
    {
        mlx_destroy_display(game->mlx);
        free(game->mlx);
    }
    #endif
    
    exit(0);
}
```

## 3. The Makefile Flags

Your `Makefile` needs to link different libraries depending on the OS. You can detect the OS using `uname -s`.

```makefile
OS = $(shell uname -s)

ifeq ($(OS), Linux)
	MINILIBX = -L minilibx-linux -lmlx -lXext -lX11 -lm -lz
else
	MINILIBX = -L minilibx_mac -lmlx -framework OpenGL -framework AppKit
endif
```

By adhering to these 3 principles, your `cub3d` binary will natively compile into an AppKit application on a Mac, and a flawless X11 application on a Linux cluster without a single code change.

---

## 4. Apple Silicon (M1/M2) — Rosetta 2 Compilation

The 42-provided MiniLibX for macOS is compiled as an `x86_64` binary. On Apple Silicon Macs, it runs under Rosetta 2 emulation. If `mlx_init` returns `NULL` on an M1/M2 Mac, the binary architecture mismatch is the most likely cause.

**Symptom:** `mlx_init` returns `NULL`, or the program crashes immediately with `SIGABRT` before a window opens.

**Fix:** Force x86_64 compilation prefix:
```bash
arch -x86_64 make
arch -x86_64 ./cub3d map.cub
```

Or add an explicit arch flag in your Makefile for macOS:
```makefile
ifeq ($(OS), Darwin)
    CC = arch -x86_64 cc
endif
```

---

## 5. Mouse Rotation — Event Hook Differences

The bonus mouse-look feature behaves very differently between platforms.

| Feature | macOS (AppKit) | Ubuntu (X11) |
|---------|---------------|--------------|
| Mouse move hook | `mlx_hook(win, 6, 0, mouse_move, game)` | Same event code (6) |
| Warp cursor to center | `mlx_mouse_move(game->mlx, game->win, WIN_W/2, WIN_H/2)` | Same API call |
| Hide cursor | Not natively supported in 42 MLX | `mlx_mouse_hide()` available in some Linux MLX forks |
| X11 cursor lock | Not applicable | May require `XGrabPointer` for robust lock |

**Safe cross-platform mouse warp pattern:**
```c
void center_mouse(t_game *game)
{
    mlx_mouse_move(game->mlx, game->win, WIN_W / 2, WIN_H / 2);
}
```
Call `center_mouse` at the end of every frame after reading the mouse delta, so the cursor resets to the center before the next frame's delta calculation.

---

## 6. X11 Expose Event (Ubuntu Only)

On Ubuntu, if you resize or minimize and restore the window, X11 fires an **Expose event** (event code `12`). Without handling it, the window goes black. Register a redraw hook:

```c
// Linux only — re-flush the image after an Expose event
mlx_hook(game->win, 12, 0, handle_expose, game);

int handle_expose(t_game *game)
{
    mlx_put_image_to_window(game->mlx, game->win, game->img.img_ptr, 0, 0);
    return (0);
}
```
macOS AppKit does not fire this event — AppKit handles redraws automatically. Wrapping this in `#ifdef __linux__` is the correct approach.
