# Evaluation Questions

Use these questions to prep for the oral defense with your evaluator. Both partners must be able to confidently answer these without looking at the code.

## The Parser

1. **Map Loading:** How do you handle maps with different row lengths? Does your `map[y][x]` access ever jump into uninitialized memory?
2. **Validation:** How do you enforce that the map is fully enclosed by walls? What is your algorithm for this?
3. **Double Spawns:** What happens if the `cub` file lists `N` (North) twice? What happens if it has no spawn direction at all?
4. **Color Parsing:** Walk through your process of extracting the floor/ceiling RGB value from the file and converting it into a usable integer. How do you handle commas or invalid values?

## The Renderer (Math / DDA)

5. **Camera Plane:** What is the camera plane, and how do you use it when calculating the ray direction for a specific screen column `x`?
6. **DDA Step:** In your algorithm, what is `deltaDistX` and `deltaDistY`, and why do they speed up the calculation over stepping a fixed small distance?
7. **Wall Hits:** How do you know when a ray has hit a wall (`1`), and how do you know which side (N, S, E, W) of the wall it hit?
8. **Fisheye Calculation:** What causes the fisheye effect, and what is the mathematical fix for perpendicular distance?

## Memory and Robustness

9. **Exit Path:** What happens when an invalid map path is passed to `./cub3d`? Walk through the cleanup steps.
10. **Memory Safety:** If `mlx_init()` or texture loading fails halfway through booting the engine, how do you prevent leaks from the memory previously allocated?
