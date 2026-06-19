# 03 — CPU Cache Locality

A 42 School evaluator will heavily scrutinize the way you write nested loops, specifically when drawing your blank floor and ceiling frames at the start of `render_frame`. 

If you swap the order of your `x` and `y` loops, your Frame Rate can plummet. Why? The answer lies in Hardware Architecture and **CPU Cache Locality**.

## The RAM Buffer is 1-Dimensional
Though we think of the screen as a 2D coordinate grid (X horizontally, Y vertically), the computer's memory structure is fundamentally 1-Dimensional. A 800x600 screen is just a single straight line of 480,000 pixels.

When you request the MLX image string (`img.addr`), you are receiving a pointer to the start of this 1D line. 
To draw at `[y][x]`, the engine physically jumps to `(y * 800) + x`.

## The Cache Line (Spatial Locality)
The CPU does not talk to RAM one pixel at a time. It is too slow.
When the CPU asks RAM for Pixel `[0]`, RAM actually hands the CPU a "Cache Line"—a massive, continuous chunk of memory (e.g., Pixels `[0]` through `[63]`). The CPU stores this in its ultra-fast L1 Cache.

### The Correct Loop (Iterating over X inside Y)
```c
// YES: Row-Major Traversal
for (int y = 0; y < WIN_H; y++)
{
    for (int x = 0; x < WIN_W; x++)
    {
        put_pixel(&img, x, y, color);
    }
}
```
In this loop, the CPU asks for `x=0`. RAM gives it `x=0` to `x=63`.
The loop steps to `x=1`... then `x=2`. The CPU already has these pixels in its L1 cache! It writes them instantly with zero RAM interaction. It only asks RAM for data every 64 pixels. This is called a **Cache Hit**.

### The Wrong Loop (Iterating over Y inside X)
```c
// NO: Column-Major Traversal 
for (int x = 0; x < WIN_W; x++)
{
    for (int y = 0; y < WIN_H; y++) // CACHE MISS DISASTER
    {
        put_pixel(&img, x, y, color);
    }
}
```
In this loop, the CPU asks for `(0, 0)`. RAM gives it `(0, 0)` through `(63, 0)`. 
But the inner loop immediately steps `y` instead of `x`! It now asks for `(0, 1)`. That pixel is 800 slots down the memory line. It is NOT in the L1 Cache. 

The CPU suffers a **Cache Miss**. It has to dump the cache, ask RAM for a new block starting at `y=1`. Then the loop steps `y=2`. Another **Cache Miss**. 

By iterating Columns over Rows in C, you trigger 480,000 explicit RAM stalls per frame. You are fighting the physical architecture of the motherboard.

## Implementation Rule
Whenever manipulating large sections of the screen background, Minimaps, or flat rectangles, **ALWAYS** place your `X` loop *inside* your `Y` loop. Read memory horizontally, never vertically, unless you are strictly rendering vertical Raycast columns (where vertical rendering is mathematically unavoidable).
