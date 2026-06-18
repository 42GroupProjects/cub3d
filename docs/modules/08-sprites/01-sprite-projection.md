# 01 — The Sprite Inverse Projection Matrix

Translating an entity floating in 2D Space into a specific vertical slice on the 1D Camera Plane requires reversing the matrix rotation you used when rotating the camera plane. 

This mathematical operation is directly sourced from Lode V's tutorial and the Wolfenstein 3D Engine.

## Step 1: Translate position to be relative to camera
First, find the vector pointing exactly from the player to the sprite.

```c
double sprite_x = entities[i].x - player.pos_x;
double sprite_y = entities[i].y - player.pos_y;
```

## Step 2: The Inverse Matrix Calculation
We need to multiply the `[sprite_x, sprite_y]` vector against the inverse of the `[plane, dir]` camera matrix.

```c
// The determinant is necessary to invert the matrix mathematically
double inv_det = 1.0 / (player.plane_x * player.dir_y - player.dir_x * player.plane_y);

// Calculate internal camera coordinates (X acts as the screen horizontal position factor)
double transform_x = inv_det * (player.dir_y * sprite_x - player.dir_x * sprite_y);

// transform_y is essentially the exact PERPENDICULAR DEPTH of the sprite from the camera plane
double transform_y = inv_det * (-player.plane_y * sprite_x + player.plane_x * sprite_y);
```

## Step 3: Finding the Screen Pixel Center
Now that we have the sprite's relative `transform_x` and `transform_y` coordinates, plotting it on our screen width (`SCREEN_W`) is simple pixel-ratio math:

```c
// What pixel column is the absolute center of this sprite?
int sprite_screen_x = (int)((SCREEN_W / 2) * (1 + transform_x / transform_y));
```

## Step 4: Finding Height/Width and Bounds Check
We use the perpendicular depth (`transform_y`) to scale how tall and wide the sprite border should be.

```c
// Protect against divide by zero or negative depth (behind the camera)
if (transform_y <= 0) return; 

// Calculate height of the sprite on screen
int sprite_height = abs((int)(SCREEN_H / transform_y)); 

// Calculate lowest and highest pixel to fill in current stripe
int draw_start_y = -sprite_height / 2 + SCREEN_H / 2;
if (draw_start_y < 0) draw_start_y = 0;
int draw_end_y = sprite_height / 2 + SCREEN_H / 2;
if (draw_end_y >= SCREEN_H) draw_end_y = SCREEN_H - 1;

// Calculate width of the sprite
int sprite_width = abs((int)(SCREEN_H / transform_y));
int draw_start_x = -sprite_width / 2 + sprite_screen_x;
if (draw_start_x < 0) draw_start_x = 0;
int draw_end_x = sprite_width / 2 + sprite_screen_x;
if (draw_end_x >= SCREEN_W) draw_end_x = SCREEN_W - 1;
```

With `draw_start_x` to `draw_end_x`, you now have the exact horizontal loop range on screen that the sprite spans. You will iterate through these columns and draw the vertical stripes of your XPM sprite texture!
