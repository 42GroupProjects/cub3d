# 04 — Mapping Endian Array Colors

You have a `texX` float derived from your ray hit, and a `texY` integer looping down the screen height. 

How do we copy the exact pixel color from `tex_north.addr` into the `game->img->pixels` buffer without flipping the red and blue channels?

## The Extract Function

We write a dedicated function that safely extracts a 32-bit color from an MLX image struct based on X/Y coordinates.

```c
int get_pixel_color(t_img *texture, int x, int y)
{
    // Clamp to prevent segfaults if math slightly overruns the 64x64 texture
    if (x < 0) x = 0;
    if (x >= texture->width) x = texture->width - 1;
    if (y < 0) y = 0;
    if (y >= texture->height) y = texture->height - 1;

    // Use line_len to safely calculate the byte offset
    int offset = (y * texture->line_len) + (x * (texture->bpp / 8));
    
    // Cast the char* memory address to an unsigned int pointer and dereference it
    return (*(unsigned int *)(texture->addr + offset));
}
```

## The Y-Axis Step Factor

When drawing the vertical stripe in your core engine `while(y < drawEnd)` loop, the wall on the screen might be 400 pixels tall, but the actual PNG texture is only 64 pixels tall. We must mathematically "step" through the texture slower than we step through the screen.

```c
// How much to increase the texture coordinate per screen pixel
double step = 1.0 * texture->height / lineHeight;

// Starting texture coordinate
// (We calculate an offset if the wall is taller than the screen!)
double texPos = (drawStart - SCREEN_HEIGHT / 2 + lineHeight / 2) * step;

while (y < drawEnd)
{
    // Cast the float position to an integer and mask it to prevent overflow
    int texY = (int)texPos & (texture->height - 1); // Only works if texture height is 2^n
    // OR safely use: int texY = (int)texPos;
    
    texPos += step;
    
    // Sample color
    int color = get_pixel_color(texture, texX, texY);
    
    // Write color to screen buffer
    put_pixel_safe(game->img, x, y, color);
    
    y++;
}
```
