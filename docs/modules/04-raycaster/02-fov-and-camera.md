# 02 — FOV and the Camera Plane

Unlike modern 3D projection where you define a Field of View simply as `float fov = 90.0`, the raycaster FOV is defined purely by the relationship between two vectors: the **Direction** and the **Camera Plane**.

## 1. The Mathematics of FOV
To find your current FOV in degrees based on your vectors, use trigonometry. 
Because the Plane is perpendicular to the Direction, they form a right triangle.
```
FOV = 2 * atan(length(plane) / length(dir))
```

If your direction vector is length `1.0` and your plane vector is length `0.66`:
`atan(0.66 / 1.0) = ~33.4 degrees`. 
Multiply by 2 for both sides = **~66.8 degrees FOV**. 

*This is the Wolfenstein 3D standard.*

## 2. Shooting Rays through the Plane
For every single pixel column on your screen width (`SCREEN_W`), you must fire exactly one ray. 
If your screen is 800 pixels wide, you fire 800 rays per frame.

You map the screen pixel `x` (from 0 to 799) to a mathematical coordinate `cameraX` spanning from `-1` (left extreme) to `1` (right extreme).

```c
int x = 0;
while (x < SCREEN_W)
{
    // 1. Calculate the X coordinate in camera space
    double cameraX = 2 * x / (double)SCREEN_W - 1;

    // 2. Calculate the exact mathematical direction of THIS specific ray
    double rayDirX = play->dir_x + play->plane_x * cameraX;
    double rayDirY = play->dir_y + play->plane_y * cameraX;
    
    // 3. Initiate DDA utilizing this rayDir...
    
    x++;
}
```

### Why does this work?
- If `x == 0` (Left side of screen), `cameraX == -1`. The ray takes the base direction and subtracts the full plane vector.
- If `x == 400` (Center of screen), `cameraX == 0`. The ray completely ignores the plane vector and points exactly at `dir_x`!
- If `x == 800` (Right side), `cameraX == 1`. The ray adds the full right plane vector.
