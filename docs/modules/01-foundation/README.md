# Module 01 — Engine Foundation

**Branch:** `main` | **Days:** 1–2

Your `so_long` project already solved the engine layer. This module extracts and rebuilds those patterns cleanly for cub3D.

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | This overview + exit checkpoint |
| [01-mlx-basics.md](./01-mlx-basics.md) | MLX lifecycle, hooks, fixed-timestep loop, key state |
| [02-image-buffer.md](./02-image-buffer.md) | Image creation, pixel formula, endianness, buffer architecture |
| [03-vectors-camera-plane.md](./03-vectors-camera-plane.md) | Direction vector, camera plane, rotation matrix, FOV |
| [04-map-format.md](./04-map-format.md) | `char **` grid, width normalization, coordinate system |
| [05-2d-grid-view.md](./05-2d-grid-view.md) | 2D canvas: draw map, player dot, direction arrow, debug ray |

---

## What To Reuse From `so_long`

| Pattern | so_long location | Rule |
|---------|-----------------|------|
| `t_image` struct (pixels, line_length, bpp, endian) | `so_long.h` | Rewrite with your names — same shape |
| MLX init → window → hooks → `mlx_loop` | `game_init.c`, `so_long.c` | Port this pattern exactly |
| `ft_memset` clear → draw → `mlx_put_image_to_window` | `game_loop()` | Keep this discipline |
| Key-down / key-up flag pair | `on_key_down`, `on_key_up` | Port this pattern |
| `mlx_do_key_autorepeatoff()` | `so_long.c:main()` | Always include on macOS |
| Centralized cleanup / destroy | `utils.c` | Port this discipline |
| `clock()`-based fixed-timestep, `dt = 1.0/30` | `game_loop()` | Use same pattern in cub3D |
| `cell_dim`, `offset_x/y` centering | `game_init.c:calc_cell_dim_and_offset()` | Use in Module 03 2D canvas |

**Below is the exact `so_long.c` game loop referenced in Lesson 01.** Copy the concept, rewrite the code:

```c
/* Pattern from so_long: fixed-timestep spin-wait loop */
int    game_loop(t_game *game)
{
    clock_t start;

    start = clock();
    update_game(game);        // process key flags, update state
    render_frame(game);       // draw to buffer
    while ((double)(clock() - start) / CLOCKS_PER_SEC < game->dt)
        ;                     // spin-wait until frame time elapsed
    mlx_put_image_to_window(game->mlx, game->window,
        game->img.img, 0, 0); // flush once per frame
    return (0);
}
```

## What To Leave Behind

| Pattern | Why |
|---------|-----|
| 2× upscale render pipeline | Adds complexity not needed for cub3D |
| `light_image` at 0.25× | Bonus-track polish only |
| Grid-cell integer player position | cub3D needs float `pos_x`/`pos_y` |
| Enemy list, particle system, coin/door logic | Unrelated to cub3D mandatory |

---

## Build Sequence

```
examples/00_window/        ← Day 1 AM
examples/01_framebuffer/   ← Day 1 PM
   [vector rotation test]  ← Day 2 AM (can be in framebuffer example)
examples/03_map_validation/ ← Day 2 PM (2D grid view)
```

---

## Module Exit Checkpoint

Every item below must be true before moving to Module 02:

**MLX and Buffer**
- [ ] `mlx_init` and `mlx_new_window` failures print `Error\n` and exit — no crash
- [ ] ESC and window X button both call cleanup and exit cleanly
- [ ] Frame loop uses spin-wait with `game->dt` — stable timing at 30fps
- [ ] Buffer cleared with `ft_memset` each frame — no pixel trails
- [ ] `mlx_put_image_to_window` called exactly once per frame

**Color and Pixels**
- [ ] `put_pixel` uses `line_length` correctly — no out-of-bounds writes
- [ ] `draw_rect` fills cleanly with correct color
- [ ] Color encoding produces correct R/G/B on screen (not swapped)

**Input**
- [ ] WASD and arrow keys tracked via key-down + key-up flag pair
- [ ] `mlx_do_key_autorepeatoff` is called
- [ ] ESC calls cleanup + `exit(0)` — clean exit, no leaks

**Vectors**
- [ ] `dir` and `plane` initialized correctly for all 4 spawn directions (N/S/E/W)
- [ ] `rotate_player` updates both `dir` and `plane` simultaneously
- [ ] After 360° rotation vectors are within floating-point tolerance of start

**2D Canvas**
- [ ] Map cells draw at correct screen positions
- [ ] Player dot tracks float `(pos_x, pos_y)` smoothly
- [ ] Direction arrow points correctly and rotates with keys
- [ ] WASD moves player — collision stops movement at wall cells

---

## Crisis Cards (Module 01)

| ⚠️ Symptom | Cause | Fix |
|-----------|-------|-----|
| Black window, nothing draws | `mlx_loop()` not called | Add `mlx_loop()` as the last line of `main` |
| Window opens then immediately closes | Missing `mlx_loop()` — program exits naturally | Same fix |
| Key repeats fire very fast when held | `mlx_do_key_autorepeatoff` missing | Call it before `mlx_loop()` |
| Colors appear swapped (red looks blue) | Endianness: byte packing reversed | Swap `r` and `b` in `color_from_rgb` |
| Pixel trail left on screen each frame | No `ft_memset` clear at frame start | Clear with `ft_memset(pixels, 0, h * line_length)` |
| Camera plane drifts and gets longer | Forgot to save `old_dir_x` before overwriting in rotation | Store both old values before computing new ones |
| Segfault on `mlx_destroy_image` | Destroying in wrong order or double-destroying NULL | Destroy image before window before display; null-check everything |

---

## Peer Explanation Drill

Both partners must answer these without reading code:

1. Why is `mlx_pixel_put` too slow? What do we use instead?
2. Write the formula for the byte offset of pixel `(x, y)` in a pixel buffer.
3. Name the 4 values returned by `mlx_get_data_addr`. What does `line_length` physically mean?
4. If `dir_x = 1, dir_y = 0` (facing East), what are `plane_x` and `plane_y`?
5. Why must `mlx_loop()` always be the last call in `main()`?

---

## Journal Prompt

Fill `docs/journal/daily-template.md` today:
- What one pattern did you reuse from `so_long` and what one thing did you rewrite?
- Paste the code of your `put_pixel` function and explain why each line is necessary.
- What was the first frame your colored rectangle appeared? Screenshot or describe.
