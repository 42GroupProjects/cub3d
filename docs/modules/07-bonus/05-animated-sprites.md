# 05 — Introduction to Animated Sprites

Rendering dynamic entities (like enemies, items, and decorations) requires a drastically different approach from drawing walls via DDA. Walls are rigid grids, but entities are mathematically floating free in 2D space.

## The Overview
To render sprites correctly, we need to execute four distinct phases during our game loop, specifically *after* DDA raycasting has finished drawing the walls.

### 1. Distance Sorting (Painter's Algorithm)
If you have a Duck entity at 5 meters away, and a Tree entity at 10 meters away, you must draw the Tree first, and the Duck second. If you draw the Duck first, the Tree will overwrite it and appear to be "in front," breaking perspective.

Every single frame, you must compute the Euclidean distance from the Player to every entity, and sort your entity array from **farthest to closest**.

### 2. Camera Inverse Projection
Once sorted, you can't just draw the sprite on screen. You need to map its 2D world `(X, Y)` position into the camera's viewport space. You must use a mathematical Inverse Camera Matrix to figure out exactly which screen pixel column the sprite lives on, and what its perceived depth is.

*(See Module 08 for the full math breakdown on this).*

### 3. Z-Buffer Depth Check
We know our Duck is closer than the Tree, but what if the Duck is standing behind a wall? Because sprites are drawn *after* the walls, drawing our Duck blindly will cause it to magically appear in front of the brick wall it is supposed to be hiding behind.

To solve this, we introduce a 1D array called the **Z-Buffer** (or Depth Buffer), which tracks the exact distance to the wall at every pixel column. If the Duck's distance is greater than the wall's distance at column `X`, we don't draw the Duck pixel at column `X`.

*(See Module 08 for the full Z-buffer breakdown).*

### 4. Animating with Delta Time
To make the Sprite cycle through an animation loop (like waking up), you must switch out the loaded XPM texture based on a timer.

```c
// Example pseudo-logic for cycling an animation based on time
entity->anim_timer += delta_time;

if (entity->anim_timer > 0.2f) // Switch frames every 0.2 seconds
{
    entity->current_frame++;
    if (entity->current_frame >= MAX_IDLE_FRAMES)
        entity->current_frame = 0; // Loop back
    
    entity->anim_timer = 0.0f; // Reset our timer
}
```
