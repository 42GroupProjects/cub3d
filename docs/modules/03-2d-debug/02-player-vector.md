# 02 — The Player Vector Mathematics

In a true 3D space, the player does not have a single integer position like `map[y][x]`. They exist at a floating-point coordinate `(2.52, 4.89)`. But position is only half the equation—you also need to know which way they are looking.

## 1. The Starting Vectors

When you parse exactly one 'N', 'S', 'E', or 'W' from the map, you initialize two mathematical vectors. 

### The Direction Vector (`dir_x`, `dir_y`)
This vector points exactly 1.0 unit forward from the player's center.
- If facing **North**: `dirX = 0`, `dirY = -1` (Negative because Y goes UP the screen)
- If facing **East**:  `dirX = 1`, `dirY = 0`
- If facing **South**: `dirX = 0`, `dirY = 1`
- If facing **West**:  `dirX = -1`, `dirY = 0`

### The Camera Plane Vector (`plane_x`, `plane_y`)
This vector represents the 2D "projection screen" floating in front of the player. It must be strictly perpendicular to the Direction vector. The length of this vector controls your Field of View (FOV).
- If facing **North**, the plane is horizontal: `planeX = 0.66`, `planeY = 0`
- If facing **East**, the plane is vertical: `planeX = 0`, `planeY = 0.66`

*(A length of 0.66 gives a perfect 66-degree FOV, ideal for this engine).*

## 2. Converting Vectors to Grid Pixel Coordinates

To draw the player on your 2D Top-Down canvas, you multiply their float position by your cell width.

```c
// Calculate pixel center
int center_x = map_offset_x + (game->pos_x * cell_dim);
int center_y = map_offset_y + (game->pos_y * cell_dim);

// Draw a simple 4x4 pixel dot
draw_rect(game->img, center_x - 2, center_y - 2, 4, 4, 0xFF0000);
```

To draw the point exactly 1.0 unit in front of the player (the endpoint of the direction vector):
```c
int dir_pixel_x = center_x + (game->dir_x * cell_dim);
int dir_pixel_y = center_y + (game->dir_y * cell_dim);
```
