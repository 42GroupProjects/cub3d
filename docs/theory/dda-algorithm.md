# Theory: The DDA Algorithm

DDA stands for Digital Differential Analysis. 
If we want to cast a ray across a grid to find a wall, we could simply take a tiny 0.01 step forward, check the map, take another 0.01 step forward, check the map.

## 0. Why NOT Naive Stepping? (The Obvious Wrong Answer)

Before DDA, the intuitive approach is to march the ray forward in tiny increments until it hits something.

### ❌ The Naive Stepping Implementation

```c
/* NAIVE APPROACH — Do NOT use this */
void    cast_ray_naive(t_game *game, double dirX, double dirY)
{
    double  ray_x;
    double  ray_y;
    double  step;
    int     map_x;
    int     map_y;

    step = 0.01;    // Tiny march step
    ray_x = game->pos_x;
    ray_y = game->pos_y;
    while (1)
    {
        ray_x += dirX * step;   // Step forward by 0.01
        ray_y += dirY * step;
        map_x = (int)ray_x;
        map_y = (int)ray_y;
        if (game->map[map_y][map_x] == '1')
            break ; // Found a wall!
    }
}
```

### Why It Breaks

**Problem 1: Catastrophic Performance**

The map might be 24 tiles wide. A ray travelling diagonally might cross `~34` grid units before hitting a wall. With step size `0.01`, that is **3,400 map lookups per ray**.

| Metric | Value |
|--------|-------|
| Steps per ray | ~3,400 |
| Rays per frame | 800 |
| **Map lookups per frame** | **~2,720,000** |
| At 60 FPS | **~163,200,000 array reads/sec** |

DDA reduces this to ~12–30 map lookups per ray (one per grid line crossed) by jumping directly to intersections.

**Problem 2: Wall Corner Phasing**

If a wall is only 1 tile wide (which most walls are), a step of `0.01` might step cleanly over the corner:
```
Step at 3.99 → Floor tile (0)
Step at 4.00 → Floor tile (0)   ← steps exactly across a 1-tile-wide wall corner
Step at 4.01 → Floor tile (0)
```
The ray phases through the wall as if it doesn't exist. Reducing to `0.001` is 10× worse performance. There is no safe step size.

**Problem 3: Floating-Point Drift**

Adding `0.01` thousands of times accumulates floating-point rounding error. After 3,000 steps, `ray_x` might be `4.99999997` instead of exactly `5.0`, causing subtle off-by-one tile misreads.

### ✅ DDA: Only Checks Grid-Line Intersections

DDA skips all of the empty space inside grid cells and jumps directly from one grid-line crossing to the next:
```
   Naive: checks 3,400 positions per ray
   DDA:   checks 12-30 positions per ray (only at grid intersections)
```


## 1. Delta Distance (`deltaDist`)
How much farther does the ray have to travel (hypotenuse-wise) to pass exactly 1 full grid unit completely?
- If the ray angle is shallow along the X axis, it will have to tear across 10 units of X to inch up 1 unit of Y.
- Delta Distance relies strictly on Pythagoras.

```c
deltaDistX = fabs(1.0 / rayDirX);
deltaDistY = fabs(1.0 / rayDirY);
```
*(If `rayDirX == 0`, `deltaDistX = 1e30` to prevent division-by-zero crashes).*

## 2. Side Distance Initialization (`sideDist`)
`deltaDist` tells us the spacing *between* grid lines. But where is the *first* grid line relative to the player?
If the player is standing at `X: 4.8`, the next grid line to the East is `5.0`. The distance is `0.2` units.
If they are facing West, the next grid line is `4.0`. The distance is `0.8` units.

```c
// If Ray is pointing WEST (reducing X index)
if (rayDirX < 0) {
    stepX = -1;
    sideDistX = (playerPosX - mapGridX) * deltaDistX;
}
// If Ray is pointing EAST (increasing X index)
else {
    stepX = 1;
    sideDistX = (mapGridX + 1.0 - playerPosX) * deltaDistX;
}
```

## 3. The Leapforge Loop (The `While` Block)
Now we have two measuring tapes extending from the player:
- `sideDistX` is currently 0.8 meters long.
- `sideDistY` is currently 1.2 meters long.

DDA says: "Whichever measuring tape is shorter, follow it until you cross the line."

```c
while (hit == 0)
{
    if (sideDistX < sideDistY) {
        // We reached an X-wall before a Y-wall
        sideDistX += deltaDistX; // Expand the tape
        mapX += stepX;           // Move inside the box
        side = 0;
    } else {
        // We reached a Y-wall before an X-wall
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
    }
    
    // Have we hit concrete?
    if (map[mapY][mapX] == 1) hit = 1;
}
```

```mermaid
flowchart TD
    Start[Start DDA Loop] --> Condition{hit == 0?}
    Condition -- Yes --> CheckDist{sideDistX < sideDistY?}
    Condition -- No --> End[Exit Loop, Wall Hit!]
    
    CheckDist -- Yes --> ExpandX[sideDistX += deltaDistX<br/>mapX += stepX<br/>side = 0]
    CheckDist -- No --> ExpandY[sideDistY += deltaDistY<br/>mapY += stepY<br/>side = 1]
    
    ExpandX --> CheckMap
    ExpandY --> CheckMap
    
    CheckMap{map[mapY][mapX] == '1'?}
    CheckMap -- Yes --> SetHit[hit = 1]
    CheckMap -- No --> Condition
    
    SetHit --> Condition
```


## 4. Perpendicular Projection Fix
Once we smack into `hit == 1`, we cannot just return Euclidean distance. If you measure hypotenuse distance to a flat wall directly in front of you, the rays bouncing off the edges of your vision had to travel "further" algebraically than the ray hitting the center of your nose. 
This makes flat walls look curved like a fishbowl.

We must flatten the distance so only the depth perpendicular to the camera plane is returned.
```c
// The magic formula
if (side == 0) // Hit an East/West wall
    perpDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
else          // Hit a North/South wall
    perpDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;
```
