# 03 — Fisheye Correction

If you calculate the Euclidean distance (the true straight-line distance from the player point to the wall hit point), and use that distance to scale the height of your drawn wall line, your game will look like you are staring through a peephole.

## Why Fisheye Happens

Imagine you are standing 5 meters away from a perfectly flat wall, staring straight ahead at it.
- The distance from your eye to the wall directly in front of you is 5m.
- But if you look 33 degrees to your left (still keeping your body still), the distance from your eye to that new point on the flat wall might be 6m. 

If you use `6m` to calculate the height of the wall strip on your screen, that part of the wall will be drawn smaller. The result? The perfectly flat wall looks like a curved dome snapping away from you.

### ❌ Using Euclidean Distance (Produces Fisheye)

This is what a first-attempt implementation looks like:

```c
/* WRONG — causes fisheye distortion */
void    draw_wall_wrong(t_game *game, int x, double ray_dir_x, double ray_dir_y)
{
    double  dist;
    int     line_height;

    // Pythagorean distance from player to the wall hit point
    dist = sqrt(
        (game->wall_hit_x - game->pos_x) * (game->wall_hit_x - game->pos_x) +
        (game->wall_hit_y - game->pos_y) * (game->wall_hit_y - game->pos_y)
    );
    // Rays at the screen edges have a LARGER dist than center rays hitting the SAME flat wall!
    line_height = (int)(SCREEN_H / dist);
    draw_vertical_line(x, line_height);
}
```

**The visual effect:**

```
Looking at a flat wall — center vs edge ray distances:

           Wall (flat, horizontal)
  +-----------+-----------+
  |           |           |
  |    5.0m   |    6.1m   |
  |  (center) |  (edge)   |
  |           |           |
              Player

Center column draws line_height = 600/5.0 = 120px  ← tall
Edge column draws   line_height = 600/6.1 =  98px  ← shorter

Result on screen:
  ████████████
  ██        ██   ← edges look shorter = curved dome appearance
  ██        ██
```

### ✅ Using Perpendicular Distance (Correct)

The perpendicular distance flattens the measurement to only the depth coordinate relative to the camera plane, ignoring the horizontal spread entirely:

```
           Wall (flat, horizontal)
  +-----------+
  |           |
  |←— 5.0m —→|   ← ALL rays return 5.0m (perpendicular to camera plane)
  |           |
  
  Player + Camera Plane (vertical bar in front of player)
```

Every column of the same flat wall gets the same `perpWallDist`, and therefore the same `lineHeight`. The wall appears flat.

You do not want the true distance to the player; you want the **true distance to the Camera Plane**. Because the Camera Plane is parallel to the wall, any intersection along its vector will result in a perfectly uniform distance (e.g., 5m everywhere).

### The Formula
After your DDA loop `while (hit == 0)` finds a wall tile, note which side of the grid triggered the hit.

```c
double perpWallDist;

// If we hit a vertical grid line (East/West wall)
if (side == 0) 
    perpWallDist = (map_x - play->pos_x + (1 - step_x) / 2) / rayDirX;
    
// If we hit a horizontal grid line (North/South wall)
else           
    perpWallDist = (map_y - play->pos_y + (1 - step_y) / 2) / rayDirY;
```

**Note:** If `rayDirX` or `rayDirY` are very close to 0, this division works fine mathematically in C because IEEE 754 floats handle division by zero as Infinity, and `map_x - pos_x` will also mathematically zero out gracefully in contexts where that direction wasn't actually moving.
