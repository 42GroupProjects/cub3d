# 03 — Introduction to the Z-Buffer

When the walls finish drawing, the screen is filled. 
When the sprite pipeline starts, it does not know where the walls are. If a Duck is standing *behind* a wall, but conceptually within the camera's FOV cone, the sprite code will calculate its pixel data and draw it *over* the wall. 

This ruins the 3D illusion entirely. We prevent this using a **Z-Buffer** (Depth Buffer).

## The Theory
We don't need a 2D Z-buffer tracking every pixel on the screen (like modern OpenGL does) because vertically, walls are uniformly distant in a raycaster. We only need a **1D array**.

For an 800-pixel wide screen, we need an array of 800 `double` floats. 
When the DDA algorithm hits a wall at screen column `x=450` and determines the perpendicular distance is `12.5`, we save `ZBuffer[450] = 12.5;`.

Later, when drawing the Duck sprite, if the duck is crossing screen column `x=450` but its `transform_y` (perpendicular space depth) is `15.0`, we know `15.0 > 12.5`. The duck is farther away than the wall at that pixel column. We **skip drawing that vertical stripe of the duck**.

```mermaid
flowchart TD
    Start[Start Sprite Pipeline] --> CalcDist[Calculate Distance to all Entities]
    CalcDist --> Sort[Quicksort Entity Array<br/>from Furthest to Closest]
    Sort --> ForEach[For Each Entity in Array:]
    
    ForEach --> Inverse[Apply Matrix Inversion<br/>Calculate transformX / transformY]
    Inverse --> MapScreen[Map to Screen Bounds<br/>draw_start_x to draw_end_x]
    
    MapScreen --> DrawStripe{Is transform_y <br/>LESS than Zbuffer[X]?}
    DrawStripe -- Yes --> RenderStripe[Render Vertical Stripe] --> NextStripe[Next X]
    DrawStripe -- No --> SkipStripe[Sprite is behind wall!<br/>Skip Stripe] --> NextStripe
    
    NextStripe --> ForEach
```

## The C Implementation

### 1. Declaring the Z-Buffer Array
In your main engine state loop, allocate your array.
```c
// At the top of your game structure
double zBuffer[SCREEN_W];
```

### 2. Writing to the Z-Buffer (During Raycasting)
Inside your main DDA loop (`04-raycaster` code), you already calculate the perpendicular distance to the wall. At the end of the `while (x < SCREEN_W)` loop, just log it.

```c
// ... end of your DDA projection ... //
perpWallDist = (hit == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);

// NEW: Save the depth to the buffer
game->zBuffer[x] = perpWallDist;

// draw the wall stripe at x...
```

### 3. Reading the Z-Buffer (During Sprite Rendering)
When calculating your `draw_start_x` and `draw_end_x` inner loops to blit the sprite texture onto the screen:

```c
for (int stripe = draw_start_x; stripe < draw_end_x; stripe++)
{
    int tex_x = (int)(256 * (stripe - (-sprite_width / 2 + sprite_screen_x)) * tex_width / sprite_width) / 256;
    
    // IMPORTANT DEPTH CHECK HERE:
    // 1) It's in front of camera plane so you don't see things behind you
    // 2) It's on the screen (left)
    // 3) It's on the screen (right)
    // 4) ZBuffer, with perpendicular distance
    if (transform_y > 0 && stripe > 0 && stripe < SCREEN_W && transform_y < game->zBuffer[stripe])
    {
        for (int y = draw_start_y; y < draw_end_y; y++) // loop through every pixel of the current stripe
        {
            // Calculate texture Y and read color
            // If color is not completely transparent (black or alpha 0)...
            put_pixel(game->img, stripe, y, color);
        }
    }
}
```

Notice the crucial line: `transform_y < game->zBuffer[stripe]`. If the sprite's distance is less than the wall's distance, it draws!
