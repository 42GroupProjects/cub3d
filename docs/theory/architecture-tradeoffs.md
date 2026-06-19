# Theory: Architecture & Trade-Offs

When building a 3D Graphics Engine in C for the first time, you must make critical decisions regarding Data Structures and Algorithms. The choices you make will directly dictate whether your engine runs at a buttery 60 FPS or grinds to a 5 FPS halt, and whether you pass the strict 42 School memory leak evaluations.

This document breaks down the viable options for each major system so you can make informed engineering decisions.

---

## 1. Finding Walls (The Raycasting Algorithm)

How do we actually send an invisible laser beam across the map to find a wall?

| Algorithm | How it Works | Pros | Cons / Trade-offs |
|-----------|--------------|------|-------------------|
| **DDA (Digital Differential Analyzer)**<br/>*(Recommended)* | Uses algebraic geometry to jump the laser beam *exactly* from grid-line to grid-line without checking the empty space inside the square. | • Mathematically perfect.<br/>• State-of-the-art for Wolfenstein 3D.<br/>• Lightning fast `O(N)` where N is grid lines crossed. | • Conceptually very hard to understand initially.<br/>• Requires computing `deltaDist` and `sideDist`. |
| **Naive Euclidean Stepping** | You increment the ray's X/Y by `0.01` in a `while` loop until it hits a `1` in the map array. | • Extremely easy to code and understand. | • Appalling CPU performance.<br/>• Can phase through the corners of walls if the step size isn't infinitely small. |
| **Bresenham's Line Drawing** | An integer-based 2D algorithm that tracks exact pixel grids to draw a continuous line. | • Perfect for plotting 2D rays on a minimap (Lab 04). | • Very bad for calculating the exact floating-point perpendicular depth required for a 3D camera plane. |

---

## 2. Rendering Pixels to the Screen

If you open an 800x600 window, you must render 480,000 pixels every single frame. At 60 FPS, that is **28,800,000 pixels calculated per second**.

| Data Structure / API | How it Works | Pros | Cons / Trade-offs |
|----------------------|--------------|------|-------------------|
| **Direct RAM Buffer Writing**<br/>*(Recommended)* | You request a 1D `char *` string from MLX. You calculate the 2D offset (`Y * line_len + X * bpp/8`) and write the integer color directly into RAM. At the end of the frame, you push the image to the window ONCE. | • Zero system call overhead.<br/>• Flawless 60 FPS performance.<br/>• Allows for post-processing effects. | • You must manually cast the `char *` to an `unsigned int *` to write hex colors.<br/>• Writing out-of-bounds causes an instant Segfault. |
| **mlx_pixel_put()** | You call the native `mlx` function to draw a pixel on the window. | • Built-in, safe. | • **DISASTER.** It executes a system call to the OS Graphics buffer for *every single pixel*. Your game will stutter at 1 FPS. |

---

## 3. Map Validation (Edge-Leak Detection)

The map must be perfectly enclosed by walls (`1`). How do we verify this?

| Algorithm | How it Works | Pros | Cons / Trade-offs |
|-----------|--------------|------|-------------------|
| **DFS Flood Fill**<br/>*(Recommended)* | A recursive Depth-First Search algorithm that spreads outwards from the player in 4 directions like water until it hits a wall or escapes the map. | • Elegant and compliant with the 25-line limit.<br/>• Unbeatable for bizarre, non-rectangular geometry. | • Can trigger a **Stack Overflow** crash if the map is absolutely massive (e.g. 1000x1000 floor spaces) due to heavy recursion. |
| **Edge-Scanning (Perimeter Check)** | Iterating over row 0, the last row, column 0, and the last column explicitly locking for spaces outside `1`. | • Requires zero recursion, memory-safe. | • Completely fails if the map is oddly shaped or has complex internal corridors that leak into dead space. |

---

## 4. Map Storage (RAM Management)

When parsing `.cub` files via `get_next_line`, you do not know how big the map will be at the start.

| Data Structure | How it Works | Pros | Cons / Trade-offs |
|----------------|--------------|------|-------------------|
| **Linked List ➞ 2D Array**<br/>*(Recommended)* | Parse the file into a Linked List node-by-node (because lists can grow infinitely). Identify `max_width` and `height`, then allocate a perfect `char **` array and copy the data over. Free the List. | • Fully dynamic.<br/>• Array provides `O(1)` contiguous lookup speed for rendering, which is mandatory for game loops. | • Requires writing a Linked-List library.<br/>• Copying step adds a small memory overhead during loading. |
| **Re-allocation (ft_realloc)** | Using `malloc` to expand a string, copying the old string, freeing the list, and appending. | • No distinct data structures needed. | • Horrendously slow and fragmenting if done 500 times for a large map. Memory leak central. |
| **Pure Linked List** | Storing the map permanently as a List and traversing it during rendering. | • Easy to construct. | • **Unplayable.** Accessing `map[Y][X]` in a Linked List requires `O(N)` traversal, doing this 800 times a frame will freeze the engine. |

---

## 5. Sprite & Entity Rendering (Combat / Enemies)

If you are implementing the Advanced Combat Lab (Phase 9) with enemy ducks, they must be drawn *over* the walls but *behind* closer walls.

| Algorithm / Structure | How it Works | Pros | Cons / Trade-offs |
|-----------------------|--------------|------|-------------------|
| **Static Array + 1D Z-Buffer**<br/>*(Recommended)* | Store 800 depth measures (1 per screen ray). When drawing a Sprite column, check if `sprite_dist < zbuffer[column]`. Store entities in a fixed `t_entity entities[50]`. | • O(N) sprite draw speed.<br/>• Zero dynamic `malloc` overhead during active gameplay (no memory fragmentation). | • Capped max entities (e.g. 50 limit).<br/>• Complex matrix inverse mathematics. |
| **Painter's Algorithm (Sorting)** | Calculate the distance of every sprite. `qsort` them from furthest to closest. Draw them in order so closer sprites overwrite further ones. | • Easy to conceptualize. | • Still requires a Z-Buffer to prevent sprites from drawing through walls!<br/>• Sorting 50 objects every single frame adds CPU overhead. |
| **Linked List Entities** | `malloc`ing a new Duck node when it spawns, and `free`ing it when it dies. | • Infinite enemies. | • Calling `malloc` during a heavy 60FPS render loop is widely considered an anti-pattern in Engine Architecture. It introduces micro-stutters and memory fragmentation over time. |

---

## 6. Trigonometric Function Calls (Performance)

Inside the game loop, the player rotation applies `cos()` and `sin()` every frame. With mouse-look, this runs for every frame.

| Approach | How it Works | Pros | Cons / Trade-offs |
|----------|--------------|------|-------------------|
| **Runtime `<math.h>`** *(Simple)* | Call `cos(angle)` and `sin(angle)` directly during the game loop. | • Zero setup. Easy to understand. | • Taylor Series computed live per call. Degrades under heavy combined load (many entities rotating together). |
| **Pre-computed Lookup Table** *(Advanced)* | Boot-time: compute `sin` and `cos` for all angles into two `double[]` arrays. Runtime: index directly by integer angle. | • O(1) memory read — fastest possible. | • Locked to fixed angular precision (e.g. 0.1°). Requires storing player angle as integer index rather than float. |

---

## 7. Texture Coordinate Wrapping

When sampling a repeating texture 480,000 times per frame, forcing the Y coordinate to stay within `[0, TEX_H)` is unavoidable.

| Approach | How it Works | Pros | Cons / Trade-offs |
|----------|--------------|------|-------------------|
| **Modulo `%`** *(Simple)* | `tex_y = (int)tex_pos % TEX_H;` | • Readable. Works for any texture size. | • Integer division is one of the slowest CPU operations. Inside the inner render loop, this runs ~480,000 times per frame. |
| **Bitwise AND `&`** *(Advanced)* | `tex_y = (int)tex_pos & (TEX_H - 1);` | • Single-cycle CPU instruction. Fastest possible wrapping. | • **Only works when `TEX_H` is a power of 2** (64, 128, 256, etc.). Non-power-of-2 textures produce corrupt rendering. |

---

## 8. Animated Door State Storage

The 07-bonus door mechanic requires tracking each door's animation position independently.

| Data Structure | How it Works | Pros | Cons / Trade-offs |
|----------------|--------------|------|-------------------|
| **`char` in `map[][]`** *(Naive)* | Use `'D'` for closed, `'O'` for open in the 2D grid. | • No extra memory. | • Only tracks binary state. Cannot track animation progress (0.0–1.0). Cannot store multiple doors at different animation stages. |
| **Static `t_door[]` Array** *(Recommended)* | Pre-allocate `t_door doors[MAX_DOORS]` at init. Each `t_door` holds `x, y, state, open_p (float)`. | • $O(1)$ access. No `malloc` in loop. Fixed memory footprint. | • Requires scanning the map at startup to populate the array. Maximum door count is fixed at compile time. |
| **Linked List of Doors** | `malloc` a new node per door at init, traverse at runtime. | • Handles any number of doors. | • $O(N)$ traversal for door lookup per frame. Unnecessary complexity for a game with 5–10 doors. |
