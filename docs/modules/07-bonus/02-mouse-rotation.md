# 02 — Free Mouse Rotation

Implementing mouse rotation requires dealing with MLX's limitations: to create an FPS-style continuous look, the mouse cannot hit the edge of the monitor screen and stop.

## The Theory
We lock the mouse programmatically to the exact center of our window. Every frame, we check how many pixels the mouse drifted away from that center point. We use that **drift (delta)** to rotate the player, and then physically teleport the mouse back to the center before the next frame.

## The C Implementation

```c
// How much one pixel of mouse movement rotates the camera (in radians)
#define MOUSE_SENSITIVITY 0.003

void mouse_look(t_game *game)
{
    int mouse_x;
    int mouse_y;
    int center_x = SCREEN_W / 2;
    int center_y = SCREEN_H / 2;
    int delta_x;

    // 1. Get current mouse position
    mlx_mouse_get_pos(game->win, &mouse_x, &mouse_y);

    // 2. Calculate horizontal drift from center
    delta_x = mouse_x - center_x;

    // 3. Apply rotation if it moved
    if (delta_x != 0)
    {
        // Multiply pixel delta by sensitivity to get rotation angle
        double angle = delta_x * MOUSE_SENSITIVITY;

        // Apply your standard rotation matrix logic here...
        rotate_vectors(game, angle); 
    }

    // 4. Teleport mouse back to center immediately
    mlx_mouse_move(game->win, center_x, center_y);
}
```

> [!WARNING]
> You must skip the **first** frame calculation, or initialize things properly. When the program launches, your OS cursor might be far from the window center. If you immediately try to center it and apply a delta, the player view will whip violently by 900+ pixels. Use a `has_initialized` boolean to ignore the delta on frame 0.

## Hiding the Cursor
To complete the FPS look, use `mlx_mouse_hide()` shortly after your window opens. Keep in mind not all MLX variations implement hiding properly.
