# 01 — Player-Centric Minimap

The problem with drawing the entire 2D map in the corner of your screen is that huge maps will shrink your cells down to 1 pixel. To solve this, professional games use a **Player-Centric Minimap**, where the player dot stays perfectly still in the center, and the map moves beneath them.

## The Theory
Rather than asking "Where do I screen-draw map cell `[y][x]`?", we invert the logic. 
We ask: **"For pixel `[px][py]` in the minimap UI box, what world coordinate does it represent?"**

## The C Implementation

```c
#define MINIMAP_W 200
#define MINIMAP_H 200
#define DIST_PER_PIXEL 0.05 // Controls zoom. Larger = zoomed out.
#define MM_OFFSET_X 20      // Padding from top-left screen edge
#define MM_OFFSET_Y 20

void draw_minimap(t_game *game)
{
    int px_x, px_y;
    double world_x, world_y;
    int mm_center_x = MINIMAP_W / 2;
    int mm_center_y = MINIMAP_H / 2;

    px_y = 0;
    while (px_y < MINIMAP_H)
    {
        px_x = 0;
        while (px_x < MINIMAP_W)
        {
            // 1. Calculate World Coordinate for this specific Minimap pixel
            world_x = game->pos_x + (px_x - mm_center_x) * DIST_PER_PIXEL;
            world_y = game->pos_y + (px_y - mm_center_y) * DIST_PER_PIXEL;

            // 2. Validate bounds
            if (world_x >= 0 && world_x < game->map_width &&
                world_y >= 0 && world_y < game->map_height)
            {
                // 3. Sample the character array
                char tile = game->map[(int)world_y][(int)world_x];

                if (tile == '1')
                    put_pixel(game->img, px_x + MM_OFFSET_X, px_y + MM_OFFSET_Y, COLOR_WALL);
                else if (tile == '0')
                    put_pixel(game->img, px_x + MM_OFFSET_X, px_y + MM_OFFSET_Y, COLOR_FLOOR);
            }
            px_x++;
        }
        px_y++;
    }

    // 4. Draw Player static in the center
    draw_player_dot(game->img, mm_center_x + MM_OFFSET_X, mm_center_y + MM_OFFSET_Y);
}
```

## Explanation
1. **`world_x = pos_x + (delta_pixel) * zoom`**:
   If we are drawing the exact center pixel of the minimap (`px_x == mm_center_x`), `delta_pixel` is exactly 0. Thus `world_x = pos_x`. 
   If we are 10 pixels to the left of the center, we sample a world coordinate slightly to the mathematically left of the player.
2. By projecting every fixed pixel to a float world coordinate, we get **sub-pixel scrolling** completely for free. As `pos_x` updates by small floats, the sampling window shifts cleanly.
