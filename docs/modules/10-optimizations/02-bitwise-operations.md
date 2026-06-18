# 02 — Bitwise Operations (Avoiding Modulo)

In C programming, the Modulo operator `%` is infamous for causing performance bottlenecks in tight loops. Division and Modulo operations take notoriously long instruction cycles on the CPU compared to simple Addition or Multiplication.

In a Raycaster, we must sample a texture image repeatedly. If a wall is very tall on the screen (e.g., 800 pixels high), but our texture is only `64x64`, we step through the texture slowly. Often, our floating point position (`tex_pos`) will exceed the bounds of the texture, and we must wrap it around using modulo to extract the Y coordinate:
```c
int tex_y = (int)tex_pos % TEX_H;
```

This line runs **480,000 times per frame** (800 columns * 600 pixels).

## The Optimization: Bitwise AND
If—and *only* if—your texture dimensions are a perfect Power of 2 (e.g., 64x64, 128x128, 256x256), you can eliminate the Modulo operator entirely and hack the CPU directly using a **Bitwise AND (`&`)**.

### The Mathematics of Wrapping
Let's analyze `TEX_H = 64`.
In binary, `64` is represented as `1000000`.
If we subtract `1` from it, we get `63`, which is `0111111`.

When you perform a Bitwise AND against all `1`s, it acts exactly like a modulo restricted to that ceiling. It truncates any bit that exceeds the highest limit.

```c
// Modulo approach: Very Slow instruction cycle
int tex_y = (int)tex_pos % 64;

// Bitwise approach: Single cycle CPU execution
int tex_y = (int)tex_pos & (64 - 1); 
```

### Visualizing the Execution
Imagine `tex_pos` climbs to `65` (It has exceeded the 64 boundary and needs to wrap back to `1`).
- Binary of `65`: `1000001`
- Binary of `63`: `0111111`
- `65 & 63`:
```
  1000001
& 0111111
  -------
  0000001 == 1 (The exact same result as 65 % 64!)
```

## Implementation Rule
You will find this optimization extensively in professional graphics engines, and you can see it deployed identically in `examples/06_texture_column/main.c`:
```c
// Example 06 implementation:
tex_y = (int)tex_pos & (TEX_H - 1);
```
**Warning:** This *only* works if `TEX_H` is exactly a power of 2. If the student uses a 50x50 texture, the bitwise hack will utterly destroy the render mapping because `49` in binary is `110001` (it misses continuous `1` coverage). Ensure textures remain powers of 2.
