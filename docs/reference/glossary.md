# Glossary & Concepts

A dictionary of graphics and rendering programming concepts necessary to pass the 42 evaluation drill.

## Graphics Rendering Concepts

- **Alpha Blending**: The process of combining a translucent foreground color with a background color. Essential for making smooth edges on weapon overlays or transparent windows. E.g., `(Foreground * Alpha) + (Background * (1 - Alpha))`.
- **Double Buffering**: A technique where you do all your mathematical pixel drawing on an invisible "offscreen" image buffer in RAM. Once the frame is 100% complete, you swap or flush that entire image to the monitor at once. This prevents Screen Tearing.
- **Euclidean Distance**: The standard point-A to point-B "straight line" distance calculated via the Pythagorean theorem `sqrt(x^2 + y^2)`. In raycasting, using this directly causes the fisheye effect.
- **Field of View (FOV)**: The angle of the observable world seen at any given moment. In our vector-based DDA engine, it is defined by the length of the Camera Plane relative to the Direction Vector (`2 * atan(plane_length / dir_length)`).
- **Fisheye Distortion**: The curving effect produced when standard Euclidean distance is used for screen projection. A result of ray divergence at the edges of the FOV. Corrected using Perpendicular Distance.
- **Frame Rate Independence (`dt`)**: Tying game logic (movement speed, animation speed) to the real-world time passed between the last loop frame and the current loop frame. Without `dt`, a fast computer runs the game at 500x speed.
- **Painter's Algorithm**: Rendering objects from back-to-front. You draw the farthest sprites first and the closest sprites last, allowing the closer ones to naturally "paint over" the further ones.
- **Perpendicular Distance**: The length of a line segment that is strictly perpendicular to the camera plane up to the point the ray struck an object. This is the **only** distance that should be used to calculate wall heights.
- **Z-Buffer (Depth Buffer)**: An array storing the physical distance to the wall at every pixel column. When drawing sprites, you check the z-buffer to prevent drawing a sprite pixel if it is further away than the wall at that same column.

## Systems & Architecture

- **Endianness**: The order in which a computer architecture stores the bytes of a multi-byte integer. MacOS is typically little-endian. This dictates whether a 32-bit hex color is stored in memory as `A R G B` or `B G R A`.
- **Finite State Machine (FSM)**: A design pattern where an entity (Enemy, Weapon, Door) exists in exactly one State at a time (e.g., `IDLE`, `WALKING`, `ATTACKING`). Transitions between states happen based on defined conditions.
- **MiniLibX (MLX)**: A minimal graphics library wrapper provided by 42 (built over X11 or macOS AppKit/OpenGL). It provides basic window management, image creation, and hook listening.
- **Screen Tearing**: A visual artifact where a display shows information from multiple frames in a single screen draw. Fixed by Double Buffering.
- **Sub-pixel Scrolling**: By tracking the player with mathematical floats (`x = 2.45`) but drawing the screen grid based on float offsets, the movement feels incredibly smooth because the pixels shift continuously, rather than snapping from grid block `2` to `3`.

## DDA Specifics

- **Camera Plane**: A 2D line segment mathematically perpendicular to the player's direction vector representing the 2D projection screen.
- **DDA (Digital Differential Analyzer)**: A fast algorithm typically used for drawing lines on grids, but specialized in 3D raycasting to find intersections across a grid space efficiently without plotting point-by-point.
- **Delta Distance**: In DDA, the slope distance a ray has to travel to jump perfectly from one `x`-grid line to the next `x`-grid line, or one `y`-grid line to the next.
- **Side Distance**: In DDA, the initial distance the ray must travel from its starting position inside a grid cell to reach the very first `x` or `y` grid intersection.

## Optimization Concepts

- **AABB (Axis-Aligned Bounding Box)**: A collision detection method where entities are bounded by rectangles whose sides are perfectly parallel to the world axes (horizontal and vertical). In Cub3D, player collision is AABB because walls are always aligned to the grid. Testing X and Y movement axes independently implements AABB sliding collision.
- **Bitwise AND `&` Wrapping**: When a power-of-2 number `N` is used as an upper boundary, `value & (N - 1)` is mathematically identical to `value % N` but executes in a single CPU clock cycle rather than a full integer division operation.
- **Cache Hit**: When the CPU requests a memory address that was already pre-loaded into the L1/L2 Cache by a previous sequential access. Cache hits are near-instantaneous. Writing pixels in Row-Major order produces mostly cache hits.
- **Cache Miss**: When the CPU requests a memory address not in L1/L2 cache, requiring a full slow round trip to main RAM. Iterating through a 2D array column-by-column produces catastrophic cache misses on every step.
- **CPU Cache / L1 Cache**: Ultra-fast on-chip memory holding recently and predictably accessed RAM addresses. Typically 32–256KB. Sequential memory access patterns (reading pixels left-to-right) exploit cache lines for massive throughput gains.
- **Frame-Rate Independence (`dt` pattern)**: Multiplying all movement, rotation, timer, and animation values by the real-world time elapsed per frame (`delta_time`). Without `dt`, a 120fps machine moves the player 4× faster than a 30fps machine.
- **Lookup Table (LUT)**: A static array pre-populated with computed values at startup, allowing $O(1)$ instantaneous retrieval rather than re-computing the value every call. Trig LUTs replace live `sin()`/`cos()` calls during the game loop.
- **Power of 2**: Numbers whose binary representation contains exactly one bit: 2, 4, 8, 16, 32, 64, 128, 256... All texture dimensions in a standard raycaster use powers of 2 to enable Bitwise AND wrapping for fast coordinate sampling.
- **Row-Major Order**: Storing and iterating 2D data by row first, then column (`grid[y][x]`), so consecutive memory reads step through adjacent bytes. The natural layout for C 2D arrays and critical for L1 Cache performance in pixel loops.
- **Taylor Series**: A mathematical technique used internally by `libm`'s `sin()` and `cos()` functions to approximate transcendental numbers as an infinite polynomial sum. Accurate but requires multiple floating-point multiply-accumulate cycles per call.

## Combat & Animation Concepts

- **Hitbox / Melee Cone**: An invisible geometric region (often a triangle or arc) in front of the player used to detect if an enemy is within striking range. Defined by a `MELEE_RANGE` (distance) and `MELEE_CONE_RAD` (half-angle).
- **Parry Window**: A brief, precisely-timed frame window — typically 150–250ms — during which the player's input successfully blocks or counters an incoming attack and triggers a `STUNNED` state on the enemy.
- **Riposte**: A counter-attack state entered immediately after a Perfect Parry. The player briefly gains an opening to deal guaranteed damage, rewarding precise defensive timing.
- **Sentinel Value**: A special marker character (e.g. `'x'`, `'V'`) written into a data structure to distinguish "already processed" cells from unvisited ones, eliminating the need for a separate boolean visited array. Used in Flood Fill (`'V'`) and the border-padding approach to map validation (`'x'`).
- **Sprite Sorting (Painter's Algorithm)**: Ordering a list of sprite entities from greatest `transformY` (farthest) to smallest (closest) before rendering. This ensures that closer sprites are drawn last and naturally overwrite the pixels of farther sprites without needing per-pixel alpha blending.
- **State Machine (FSM — Finite State Machine)**: A design pattern where an entity exists in exactly one discrete state. Transitions between states happen only when a defined condition is met. Critical for weapon animations (W_IDLE → W_WINDUP → W_ACTIVE) and enemy behavior (WADDLE → AGGRO → WINDUP → STUNNED).
- **Stun Lock**: A game mechanic where the `STUNNED` state prevents an enemy from acting or transitioning to `AGGRO` for a fixed duration, giving the player a guaranteed window to execute a follow-up attack.

## Platform-Specific Concepts

- **AppKit**: Apple's macOS UI framework used internally by the macOS version of MiniLibX. On Apple Silicon (M1/M2) Macs, the standard 42 MiniLibX may require the Rosetta 2 emulation layer to compile and run correctly.
- **Rosetta 2**: Apple's binary translation layer that allows x86_64 binaries (compiled for Intel) to run on Apple Silicon (ARM64) chips. The 42-provided MiniLibX for macOS is an x86_64 binary, so it runs under Rosetta 2 on M1/M2 Macs. Compile with `arch -x86_64` if MLX fails to initialize on Apple Silicon.
- **X11 / Xlib**: The windowing system and display server protocol used on Linux. The Linux version of MiniLibX wraps X11 calls. Unique to Linux: `mlx_destroy_display()` must be called explicitly on teardown to avoid Valgrind leaks.
- **`uname -s`**: A shell command returning the OS identifier string (`Darwin` for macOS, `Linux` for Ubuntu). Used inside Makefiles to conditionally link the correct MiniLibX library and flags for the target platform.
