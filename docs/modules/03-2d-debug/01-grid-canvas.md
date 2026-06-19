# 01 — Building the 2D Grid Canvas

Before we render 3D walls, we need a 2D topographical map. This canvas acts as our ground truth for debugging mathematically complex problems in the raycaster. 

## 1. Calculating Cell Dimensions
You don't want your map to be tiny or bleed off the screen. We must scale it to fit within a specific canvas area based on the map's width and height.

```c
// Define maximum area for the minimap
float max_canvas_w = SCREEN_W * 0.8;
float max_canvas_h = SCREEN_H * 0.8;

// Calculate the maximum possible cell size based on width and height
float cell_w = max_canvas_w / map_width;
float cell_h = max_canvas_h / map_height;

// Take the smaller of the two to ensure it fits perfectly without stretching
float cell_dim = (cell_w < cell_h) ? cell_w : cell_h;

// Calculate offset to center the map
float offset_x = (SCREEN_W - (map_width * cell_dim)) / 2;
float offset_y = (SCREEN_H - (map_height * cell_dim)) / 2;
```

## 2. Drawing the Map Cells
Loop over your 2D character array and draw a rectangle for every wall (`1`) and floor (`0`) tile using the dimensions calculated above. Leave void spaces black.

```c
int y = 0;
while (y < map_height)
{
    int x = 0;
    while (x < map_width)
    {
        int screen_x = offset_x + (x * cell_dim);
        int screen_y = offset_y + (y * cell_dim);

        if (map[y][x] == '1')
            draw_rect(img, screen_x, screen_y, cell_dim, cell_dim, ColorWall);
        else if (map[y][x] == '0' || is_spawn_char(map[y][x]))
            draw_rect(img, screen_x, screen_y, cell_dim, cell_dim, ColorFloor);
        x++;
    }
    y++;
}
```

## 3. Drawing the Player
The player's position is inherently floating-point. 
If `pos_x = 2.5` and `pos_y = 3.5`, they are exactly in the center of cell `[3][2]`.

```c
// Convert mathematical player float into a screen pixel coordinate
int px = offset_x + (int)(pos_x * cell_dim);
int py = offset_y + (int)(pos_y * cell_dim);

// Draw a small dot centered on px/py
draw_rect(img, px - 2, py - 2, 4, 4, ColorPlayer);
```

## 4. Visualizing the Direction Vector
To prove that our Rotation Matrix actually works, we draw a line coming out of the player indicating where `dir_x` and `dir_y` are pointing.

```c
// Extend the dir vector for visibility (e.g., 20 pixels long)
int end_x = px + (int)(dir_x * 20);
int end_y = py + (int)(dir_y * 20);

// Use a simple Bresenham or DDA line plotting algorithm here
draw_line(img, px, py, end_x, end_y, ColorDirection);
```
*If your arrow gets longer or shorter as you turn, it means your rotation equations are modifying `dir_x` before using the old value to calculate `dir_y`. Save your variables!*
