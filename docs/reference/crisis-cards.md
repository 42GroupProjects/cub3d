# Crisis Cards

Keep this document open while debugging. If you encounter any of these common failure states, use the card to guide your fix.

## 1. Black Screen on Startup
- **Symptoms**: Window opens but remains black. No rendering happens.
- **Fix**: Check `mlx_loop()`, verify your rendering function is actually called inside the game loop or as a hook, and ensure you're pushing the image buffer to the window using `mlx_put_image_to_window`.

## 2. Map Leaks Through Spaces
- **Symptoms**: Map looks valid, but flood fill or collision detection escapes the map bounds.
- **Fix**: Revisit validation. Spaces inside the map are effectively unplayable void but must still be enclosed by walls. Ensure your validation checks the neighbors for every floor (`0`) tile, even those adjacent to spaces.

## 3. Player Spawn Invalid or Duplicated
- **Symptoms**: Segfault at start, or player spawns outside map or inside wall.
- **Fix**: Count your spawn letters. The parser must enforce *exactly one* spawn point. Verify that the coordinates injected into the player structure map to valid `x/y` indices that aren't walls (`1`).

## 4. Fisheye Distortion
- **Symptoms**: Walls look curved instead of flat.
- **Fix**: You are using Euclidean distance. You must multiply the distance to the wall by `cos(ray_angle - player_angle)` to get the perpendicular distance.

## 5. Textures Mirrored or Stretched
- **Symptoms**: The wall texture looks stretched horizontally or flipped.
- **Fix**: Calculate your exact `wallX` hit point (where on the wall the ray hit), subtract the integer portion, and multiply by the texture width. If moving left or up, reverse the X direction for the texture rendering based on the ray's heading.

## 6. Wrong Wall Chosen for Texture
- **Symptoms**: North texture shows up on East walls.
- **Fix**: Check your step directions when the DDA hit occurs. Are you distinguishing between an X-side hit and a Y-side hit, and further distinguishing based on whether `rayDirX`/`rayDirY` are positive or negative?

## 7. Player Clips Through Corners
- **Symptoms**: When moving diagonally against an inner wall corner, the player passes through.
- **Fix**: Add a small buffer/epsilon to the player radius when checking collisions against the grid, or verify that the X and Y movement components are validated simultaneously against corner intersections.

## 8. Segfault on Close / Cleanup
- **Symptoms**: Pressing Esc or the X button crashes the program instead of a clean exit.
- **Fix**: Ensure `mlx_destroy_image`, `mlx_destroy_window`, and `mlx_destroy_display` (if applicable) are called. Free all allocated textures and map arrays. Do not double-free.

## 9. Colors Wrong (Image Memory Assumptions)
- **Symptoms**: Pure blue appears red or vice-versa.
- **Fix**: Endianness matters. The memory representation of an integer might be `A R G B` or `B G R A` depending on the system. Be explicit about bitwise shifts when writing pixels the data address.

## 10. Works on One Map, Fails on Ragged Maps
- **Symptoms**: Valid maps with uneven row lengths trigger segfaults.
- **Fix**: Normalize your 2D map array to the maximum row width or ensure strict boundary checking in your loops. Do not assume `map[y][x]` exists without `y < max_height` and `x < len(map[y])`.

---

## 🔥 Deep-Dive Diagnostic Workflows

When the rapid-fire fixes above don't work, follow these procedural workflows to isolate the true cause of a crash.

### Workflow A: The "Corner Segfault" (DDA Out-of-Bounds)
If your game crashes exclusively when you walk directly into an inner wall corner, the DDA algorithm is escaping the map bounds.
1. **The Vector Test**: Does `pos_x` perfectly equal an integer (e.g., `5.000`)? If so, the ray math hits a divide-by-zero or `1e30` fallback edge case.
2. **The Buffer Check**: Your collision system must stop the player slightly *before* they hit the integer grid line. Implement `#define COLLISION_BUFF 0.05` and apply it to your movement inputs.
3. **The Index Check**: Place a massive `if (ray_grid_x < 0 || ray_grid_x >= max_w)` trap inside your `while(hit == 0)` DDA loop. If it triggers, the collision system is failing to stop the player, allowing the raycast origin to spawn inside a solid wall, firing a ray into the negative void.

### Workflow B: The Memory Leak (Valgrind / Leaks Tool)
If the evaluation requires zero leaks and you are failing:
1. **The GNOH (Get Next Line Over-read) Trap**: If parser validation fails *while reading the middle of a file*, did you keep calling `get_next_line` to drain the buffer? No? You leaked static memory inside GNL. You must either read to EOF or write a GNL variant that accepts a `clear_buffer` flag.
2. **The MLX Double-Free**: Did you call `mlx_destroy_image` before `mlx_destroy_window`? Good. Did you call it twice? `mlx_destroy_window` sometimes frees attached resources automatically depending on the OS build.
3. **The Pointer Array (Map) Trap**: Ensure your loop for freeing the `char **map` array iterates exactly `map_height` times. If `map_height` was calculated before normalization, you might have left dangling string pointers at the bottom.

### Workflow C: The Rendering Bottleneck
If your 3D engine is running terribly slow (less than 60 FPS on a 42 Mac):
1. **The `mlx_pixel_put` test**: Search your entire codebase for this function. If it is used inside a loop, you fail. Replace it with direct image data address writing.
2. **The Resolution Test**: Are you rendering at 4K? The DDA algorithm calculates thousands of stripes. Hardcode a smaller internal rendering resolution (e.g., 640x480) and test. If the FPS skyrockets, your math is fine, your plotting is just too heavy.
3. **The Painter's Fallback**: If using Sprites, are you drawing *all* sprites every frame, even those behind the camera or occluded by walls? Implement a strict Z-Buffer check and FOV check *before* you calculate the `[y][x]` loop for the sprite's texture.
