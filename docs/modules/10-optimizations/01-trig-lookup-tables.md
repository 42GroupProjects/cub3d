# 01 — Trigonometric Lookup Tables

In the mandatory basic version of a Raycaster, every time the player presses the Left or Right arrow key to rotate the camera, we call the `cos()` and `sin()` functions from `<math.h>` to calculate the 2D rotation matrix.

If the user is using a mouse-look feature, or if you begin calculating trigonometric physics for 50 ducks moving globally around the map simultaneously, calling `<math.h>` dynamically starts to degrade performance. Functions like `sin()` calculate Taylor Series approximations on the fly, which is computationally expensive.

## The Optimization: Pre-computation

Instead of calculating `sin(angle)` hundreds of times per frame, we can calculate it exactly once when the program boots up, and store the results in an array. This transforms a CPU-heavy mathematical operation into lightning-fast $O(1)$ memory lookup.

Since there are 360 degrees in a circle (or $2\pi$ radians), we can create an array of 360 floats (or 3600 floats for 1 decimal precision).

### Step 1: Initialization
During your engine's setup phase, calculate the values and store them:
```c
// Calculate for 360 degrees, or 3600 indices if you want 0.1 degree precision
double g_sin_table[3600];
double g_cos_table[3600];

void init_trig_tables(void)
{
    int i = 0;
    while (i < 3600)
    {
        // Convert the index back to radians
        double radians = (i * 0.1) * (M_PI / 180.0);
        g_sin_table[i] = sin(radians);
        g_cos_table[i] = cos(radians);
        i++;
    }
}
```

### Step 2: Implementation in the Game Loop
Instead of saving the player's rotation as a floating-point `double angle = 1.5;`, you save their angle as an **Integer Index** pointing to the array: `int angle_index = 850;` (Representing 85.0 degrees).

When you need to rotate the camera:
```c
void rotate_player(t_player *p, int delta_index)
{
    p->angle_index += delta_index;
    
    // Wrap the index if they spin in a full circle
    if (p->angle_index >= 3600) p->angle_index -= 3600;
    if (p->angle_index < 0) p->angle_index += 3600;

    // Instant O(1) Lookup
    double rot_sin = g_sin_table[p->angle_index];
    double rot_cos = g_cos_table[p->angle_index];

    // Apply the 2D Rotation Matrix
    double oldDirX = p->dirX;
    p->dirX = p->dirX * rot_cos - p->dirY * rot_sin;
    p->dirY = oldDirX * rot_sin + p->dirY * rot_cos;
    
    double oldPlaneX = p->planeX;
    p->planeX = p->planeX * rot_cos - p->planeY * rot_sin;
    p->planeY = oldPlaneX * rot_sin + p->planeY * rot_cos;
}
```

## Trade-Off Summary
- **Pros:** Massive boost in calculation speed. Ideal for rendering heavily populated scenes or complex mouse-movement.
- **Cons:** You are permanently locked into the precision of your array. If your array step is `1.0` degrees, your camera rotation will feel inherently "choppy" because it cannot rotate `0.5` degrees. Increasing array size mitigates this at the cost of slight boot-time overhead.
