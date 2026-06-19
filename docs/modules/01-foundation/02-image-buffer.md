# 02 — Image Buffer: Pixels, Memory, and Endianness

## Outcome
You can allocate an MLX image, write pixels directly into its memory buffer, and put that image to the screen at 30fps. You understand exactly why this is faster than `mlx_pixel_put`.

---

## 1. Why Not `mlx_pixel_put`?

`mlx_pixel_put(mlx, window, x, y, color)` puts one pixel directly to the screen. This triggers a round-trip through the windowing system for **every single pixel**. At 800×600, that is 480,000 system calls per frame. At 30fps, that is 14.4 million system calls per second.

The correct approach for real-time rendering: **write into a plain memory buffer first, then flush the whole buffer to the screen in one call**.

---

## 2. The Image Struct

```c
typedef struct s_image {
    void    *img;           // MLX image handle
    char    *pixels;        // raw pixel byte array
    int     width;
    int     height;
    int     line_length;    // bytes per row (can differ from width * 4!)
    int     bits_per_pixel; // usually 32
    int     endian;         // 0 = little-endian, 1 = big-endian
} t_image;
```

Create and retrieve the buffer:

```c
t_image img;
img.img    = mlx_new_image(mlx, width, height);
img.pixels = mlx_get_data_addr(img.img,
                 &img.bits_per_pixel,
                 &img.line_length,
                 &img.endian);
img.width  = width;
img.height = height;
```

> [!IMPORTANT]
> `line_length` is set by the OS and may be padded to a multiple of 4 or 8 bytes.
> **Never assume** `line_length == width * 4`. Always use the value returned by
> `mlx_get_data_addr`.

---

## 3. Writing a Pixel

The formula to write pixel `(x, y)` into the 1D byte array `pixels`:

```
byte_offset = y * line_length + x * (bits_per_pixel / 8)
```

Since `bits_per_pixel` is almost always 32 (4 bytes):

```c
void put_pixel(t_image *img, int x, int y, unsigned int color)
{
    unsigned int *dst;

    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return ;
    dst  = (unsigned int *)(img->pixels + y * img->line_length + x * 4);
    *dst = color;
}
```

---

## 4. Color Format: Endianness

On **little-endian** systems (all modern Intel/ARM): the bytes in memory are stored `B G R 0x00` (least significant byte first), but you write them as a single 32-bit integer `0x00RRGGBB`.

```c
unsigned int color = (r << 16) | (g << 8) | b;
put_pixel(&img, x, y, color);
```

Check `img.endian`. If it is `1` (big-endian), the layout flips. In practice, MLX on macOS reports `endian = 0` for XPM images and varies for `mlx_new_image`. Always write portable code:

```c
unsigned int encode_color(int r, int g, int b, int endian)
{
    if (endian == 0) // little-endian
        return ((r << 16) | (g << 8) | b);
    else             // big-endian
        return ((b << 16) | (g << 8) | r);
}
```

---

## 5. The Dual-Buffer Architecture (from so_long)

Your `so_long` project used a smart three-layer image system:
- **`draw_image`**: a half-resolution buffer where all drawing happens.
- **`light_image`**: a quarter-resolution buffer for lighting.
- **`window_image`**: the full-resolution MLX image that goes to the screen.

For **Cub3D**, we keep it simpler but still use the same principle:

```
render_image (1280×720)   ← write all raycaster output here
      ↓
mlx_put_image_to_window() ← flush once per frame
```

Never draw directly to the window. Always:
1. Clear the buffer (`ft_memset`).
2. Draw everything.
3. Flush once with `mlx_put_image_to_window`.

```c
// Clear the image buffer at the top of each frame
ft_memset(game->render_image.pixels, 0,
    game->render_image.height * game->render_image.line_length);
```

---

## 6. Build Exercise

Extend `examples/01_framebuffer/main.c`:

**Step A — Red Square**  
Write a function `draw_rect(t_image *img, int x, int y, int w, int h, unsigned int color)`. Draw a 100×100 red square at position (50, 50).

**Step B — Gradient Background**  
Fill the entire image with a gradient: `color = (x * 255 / width) << 16 | (y * 255 / height) << 8`.

**Step C — Moving Square**  
Every frame, increment the square's `x` position by `5 * game->dt * 30`. When `x > width`, wrap to 0. Confirm it moves at the same speed regardless of frame load.

---

## Checkpoint

- [ ] Window shows a colored square, not black
- [ ] Gradient fills the entire buffer correctly
- [ ] Square moves and wraps smoothly
- [ ] Clearing with `ft_memset` each frame prevents trails
- [ ] `mlx_put_image_to_window` is called exactly once per loop iteration

## Crisis Card

| Symptom | Fix |
|---------|-----|
| Square leaves a trail | You forgot to `ft_memset` the buffer at the start of the frame. |
| Colors are wrong (B↔R swapped) | Check endianness. Try swapping `r` and `b` in your color formula. |
| Random crash when accessing pixel | You're computing the offset without `line_length` — use it! |
| Segfault writing to `pixels` | `mlx_get_data_addr` was never called, or `img` was never created. |

## Review Questions

1. Why is `line_length * height` the correct total byte count, not `width * 4 * height`?
2. What happens visually if you forget to clear the buffer each frame?
3. In the so_long architecture, why was `draw_image` rendered at half resolution before being upscaled to `window_image`?

## Journal Prompt

Write in your journal:
- The first time your square appeared on screen — how did it feel?
- Explain the memory layout of the pixel buffer to your partner without using the code
- What would happen if `line_length` was 5 instead of `width * 4`?
