# 03 — Drawing 2D Lines (Bresenham vs DDA)

To verify that your camera is turning correctly when you press Left/Right, you must draw a line on the 2D Canvas from your `center_pixel` to your `dir_pixel`. 

You cannot use `mlx_pixel_put(x, y)` in a `while` loop naively, because lines have slopes. You need a line-drawing algorithm.

## Option A: The DDA Line Algorithm
Digital Differential Analyzer (DDA) is the easiest line algorithm to implement in C. We will eventually use a 3D grid-adapted version of this for our 3D walls!

```c
void draw_line_dda(t_img *img, int x1, int y1, int x2, int y2, int color)
{
    float x = x1;
    float y = y1;
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    // Find the longest step
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    // Calculate increment per step
    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;
    
    for (int i = 0; i <= steps; i++)
    {
        put_pixel(img, round(x), round(y), color);
        x += x_inc;
        y += y_inc;
    }
}
```

## Option B: Bresenham's Line Algorithm
Bresenham is strictly integer-based. Because it doesn't use float division, it is famously the fastest way to draw lines in computer graphics history, though the implementation is slightly harder to read.

```c
void draw_line_bresenham(t_img *img, int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = (dx > dy ? dx : -dy) / 2, e2;
 
    for(;;)
    {
        put_pixel(img, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}
```

By connecting your `player_x`/`player_y` pixel to your `dir_pixel_x`/`dir_pixel_y` using one of these functions, you create the "Pointer Line" attached to the player on your 2D minimap.
