# Formulas & Mathematics

The mathematical back-end required for the engine, largely based on Lode Vandevenne's tutorial and basic trigonometry. Use this as a cheat sheet for the final evaluation.

## 1. 2D Vector Rotation Matrix

When the player turns left or right by `angle` (in radians), you apply a classic 2D rotation matrix to both the `dir` vector and the `plane` vector.

```c
// Rotating the Direction Vector
double oldDirX = dirX;
dirX = dirX * cos(angle) - dirY * sin(angle);
dirY = oldDirX * sin(angle) + dirY * cos(angle);

// Rotating the Camera Plane
double oldPlaneX = planeX;
planeX = planeX * cos(angle) - planeY * sin(angle);
planeY = oldPlaneX * sin(angle) + planeY * cos(angle);
```
*(Always save the `oldX` variable before the first line overwrites it!)*

## 2. Ray Direction Generation

For a screen column `x` (where `0` is the left edge and `W-1` is the right edge of the screen resolution `W`), you shoot a localized ray:

```c
// cameraX is the x-coordinate on the camera plane, scaled from -1 (left) to +1 (right)
double cameraX = 2 * x / (double)w - 1;

// The combined ray direction is the base direction plus a fraction of the camera plane
double rayDirX = dirX + planeX * cameraX;
double rayDirY = dirY + planeY * cameraX;
```

## 3. DDA Delta Distances

The hypotenuse distance the ray travels along its path to traverse exactly one grid unit horizontally or vertically.

```c
// How much distance to travel from an x-grid-line to next x-grid-line?
double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);

// How much distance to travel from a y-grid-line to next y-grid-line?
double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
```
*(Setting zeroes to `1e30` prevents divide-by-zero crashes).*

## 4. Perpendicular Wall Distance

When a ray strikes the wall, you MUST calculate the perpendicular distance space from the invisible camera plane to the wall to prevent fisheye distortion:

```c
// If a ray hits an X-axis wall (East/West)
double perpWallDist = (hitGridX - spawnX + (1 - stepX) / 2) / rayDirX;

// If a ray hits a Y-axis wall (North/South)
double perpWallDist = (hitGridY - spawnY + (1 - stepY) / 2) / rayDirY;
```

## 5. Line Projection (Screen Drawing)

Converting the calculated distance into physical pixel vertical heights on the screen.

```c
// Define standard height based on screen center
int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

// Where to start the line (clamped to screen bounds)
int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
if (drawStart < 0) drawStart = 0;

// Where to end the line
int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
```

## 6. Texture X Coordinate Mapping

Once a ray hits a wall, which vertical strip of the PNG texture do we draw? We mathematically find the exact float hit point on the grid.

```c
// 1. Calculate the exact float hit position
double wallX;
if (side == 0) wallX = pos_y + perpWallDist * rayDirY; // Hit X wall, Y is the intersection
else           wallX = pos_x + perpWallDist * rayDirX; // Hit Y wall, X is the intersection

// 2. Subtract the integer part to get a float strictly between 0.0 and 1.0
wallX -= floor((wallX));

// 3. Map that 0.0-1.0 float to the pixel width of the XPM texture (e.g. 64)
int texX = (int)(wallX * (double)texWidth);

// 4. Flip the texture reading left/right depending on the face normal!
if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;
```

## 7. Inverse Matrix Formula (Sprite Projection)

Projecting a 2D entity object back onto the 1D screen requires multiplying its relative coordinates by the Inverse Matrix of the Camera.

```c
// Find vector from player to sprite
double sprite_x = entity.x - player.pos_x;
double sprite_y = entity.y - player.pos_y;

// Standard Inverse Matrix Determinant Formula
// [ a b ] -1 =   1   * [  d -b ]
// [ c d ]      (ad-bc) [ -c  a ]
double invDet = 1.0 / (plane_x * dir_y - dir_x * plane_y);

// Final horizontal and depth coordinate
double transformX = invDet * (dir_y * sprite_x - dir_x * sprite_y);
double transformY = invDet * (-plane_y * sprite_x + plane_x * sprite_y); // This is Z-Depth!

// Convert horizontal float to screen pixel column
int spriteScreenX = (int)((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
```

## 8. AABB Sliding Collision

Independent axis tests that allow the player to slide along walls without snagging on corners.

```c
// Test X movement independently
int next_x = (int)(pos_x + speed_x);
if (map[map_y][next_x] == 0)
    pos_x += speed_x;

// Test Y movement independently
int next_y = (int)(pos_y + speed_y);
if (map[next_y][map_x] == 0)
    pos_y += speed_y;
```

## 9. Directional Flat Shading (Cheap Lighting)

Darken North/South-facing walls by halving their RGB values. Produces instant depth illusion for zero GPU cost.

```c
// Bit-shift the 24-bit RGB color right by 1 position to halve all three channels.
// AND with 0x7F7F7F prevents the sign bit bleeding across channel boundaries.
if (side == 1)
    color = (color >> 1) & 0x7F7F7F;
```

## 10. Depth-Based Fog (Distance Shading)

Extract, scale, and repack the individual R, G, B channels based on wall distance.

```c
double intensity = 1.0 / (1.0 + perp_dist * FOG_FACTOR);

int r = ((color >> 16) & 0xFF) * intensity;
int g = ((color >> 8)  & 0xFF) * intensity;
int b = (color & 0xFF) * intensity;

color = (r << 16) | (g << 8) | b;
```

## 11. Bitwise Texture Coordinate Wrapping

Replaces `% TEX_H` with a single CPU cycle when texture dimensions are a power of 2.

```c
// ONLY valid when TEX_H is a power of 2 (e.g. 64, 128):
int tex_y = (int)tex_pos & (TEX_H - 1);
// Binary mechanics: (TEX_H - 1) == 0b00111111 when TEX_H == 64.
// AND against all-ones mask strips any bits above the boundary,
// wrapping the value exactly as modulo would.
```

## 12. Field of View Calculation

```
FOV_degrees = 2 * atan(plane_length / dir_length) * (180 / PI)
```

For `dir_length = 1.0`:
- `plane = 0.66` → FOV ≈ 66°
- `plane = 1.0`  → FOV = 90°
- `plane = 0.5`  → FOV ≈ 53°
