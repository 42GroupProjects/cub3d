# Theory: Advanced Texture Mapping & Depth Shading

In `06_texture_column_lab`, we successfully sampled pixels from an XPM image and painted them symmetrically onto a 3D projected wall stripe. However, to bring a Raycaster to professional standards, we must understand the exact Math of extraction and apply dynamic coloring.

## 1. Extracting the Decimal Wall Hit (`wall_x`)

A Ray cast from the player will eventually strike a wall. DDA tells us exactly *which* integer grid box it hit (`mapX, mapY`). But DDA does not explicitly state the decimal offset (i.e. did the ray hit the middle of the wall, or the left edge?).

Without this offset, we have no idea which vertical column of the Texture Image to sample.

### The Algebra
To find the exact Cartesian hit coordinate, we take the Player's Position and add the Ray's Direction Vector multiplied by the total distance traveled (`perpDist`).

If we hit an X-aligned wall (East/West):
```c
double hitOffset = player.posY + (perpDist * rayDirY);
```
The result might be `4.382`. We subtract `floor(hitOffset)` (which is `4`) to get `0.382`.
This means the ray hit EXACTLY 38.2% across the wall face.

### Translating to Texture Space
If our Texture is 64 pixels wide, we simply multiply:
```c
texX = (int)(0.382 * 64.0); // Result: 24
```
We now know to sample exactly the 24th column of pixels in our texture string!

### Flipping the Coordinates
Because textures always read left-to-right, looking at the North side of a wall vs the South side of a wall will result in the texture looking mirrored horizontally in one direction.

You must catch this and invert the `texX` if facing specific compass directions:
```c
if (hit_side == 0 && rayDirX > 0) texX = 64 - texX - 1;
if (hit_side == 1 && rayDirY < 0) texX = 64 - texX - 1;
```

### ⚠️ Common Logic Mistakes in `wall_x`

**Mistake 1: Using the wrong axis for `hitOffset`**

```c
/* WRONG — always using posY regardless of which wall was hit */
double hitOffset = player.posY + (perpDist * rayDirY);
```

If `side == 0` (ray hit an East/West vertical wall), the intersection lies along the Y axis — that is correct. But if `side == 1` (ray hit a North/South horizontal wall), the intersection lies along the X axis. Using Y when you should use X produces completely wrong texture column sampling.

**Fix:** Branch on `side`:
```c
double hitOffset;

if (side == 0)
    hitOffset = player.posY + (perpDist * rayDirY); // X-wall: Y is the cross-axis
else
    hitOffset = player.posX + (perpDist * rayDirX); // Y-wall: X is the cross-axis
hitOffset -= floor(hitOffset); // Keep 0.0 - 1.0 fraction
texX = (int)(hitOffset * TEX_W);
```

**Mistake 2: Forgetting to strip the integer part**

```c
/* WRONG — hitOffset might be 4.382, so texX = 4.382 * 64 = 280 */
texX = (int)(hitOffset * 64.0); // Out-of-bounds texture read!
```

If `hitOffset` is `4.382` instead of `0.382`, you multiply to get `280` — 216 pixels beyond the 64-wide texture. You are now reading raw memory garbage as color data. Valgrind flags this as a read error.

**Fix:** Always `floor()` first:
```c
hitOffset -= floor(hitOffset); // 4.382 → 0.382
texX = (int)(hitOffset * 64.0); // 0.382 * 64 = 24 ✅
```

**Mistake 3: Not flipping `texX` — mirrored textures**

Without the flip correction, a brick texture on the NORTH face of a wall looks correct, but the SOUTH face appears horizontally mirrored. Students often miss this because it's only visible when you walk *around* a wall to look at its other side.

```
NORTH face (no flip needed): | 🧱🧱🧱🧱 |   ← correct
SOUTH face (flip needed):    | 🧱🧱🧱🧱 |   ← mirrored without the texX inversion
```


## 2. Flat Shading & Depth Blending

A scene looks very flat if a wall 20 meters away is exactly as bright as a wall 1 meter away. We can fake lighting very cheaply using bit-shifting.

### Directional Flat Shading (Cheapest)
Every 3D engine mimics a "Sun". If you darken all North/South walls slightly, they create an immediate illusion of 3D geometry alongside the lit East/West walls.
```c
if (hit_side == 1) // Apply shadow to North/South walls
{
    // Bit shift the hex color right to halve the RGB values!
    // AND with 0x7F7F7F to prevent bits from bleeding across color channels
    color = (color >> 1) & 0x7F7F7F;
}
```

### Depth-Based Fog (Advanced)
If you want walls to slowly fade into blackness the further away they are, you must implement a shading gradient. Modifying Hex colors algebraically is very slow because you must extract R, G, and B, modify them independently, and pack them back in:

```c
// CPU Heavy Gradient Example
int r = (color >> 16) & 0xFF;
int g = (color >> 8) & 0xFF;
int b = color & 0xFF;

// Apply depth dropoff (example: brightness halves every 5 units)
double intensity = 1.0 / (1.0 + (perpDist * 0.2)); 

r *= intensity;
g *= intensity;
b *= intensity;

color = (r << 16) | (g << 8) | b;
```
This looks beautiful during evaluation, but comes at a heavy 480,000-operations-per-frame CPU load unless optimized with lookup tables.
