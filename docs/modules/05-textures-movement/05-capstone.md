# Capstone 05: The Perfect Mandatory Engine

## The Objective
You must merge your mathematical solid-color Wolfenstein renderer with the four XPM textures defined by your `.cub` parser, and implement fully fluid wall-sliding movement.

## Requirements Checklist
1. [ ] Create a map file with `NO north.xpm`, `SO south.xpm`, `WE west.xpm`, `EA east.xpm`.
2. [ ] Write four unique `64x64` placeholder textures (just write letters "N", "S", "E", "W" on them using GIMP or Photoshop).
3. [ ] Load all 4 textures using `mlx_xpm_file_to_image` safely.
4. [ ] In your DDA loop, detect which wall face was hit (North, South, East, West) based on the `step_x`/`step_y` output and `hit_side (0 or 1)` determination.
5. [ ] Pass the correct `t_img *` texture into your `texX` mapping logic.
6. [ ] Hook keyboard variables to `pos_x` and `pos_y`, and protect them mathematically so moving North into a wall cancels North movement but retains East movement (Sliding).
7. [ ] **Execution:** You should now be inside a fully textured 3D maze. The textures should map perfectly to their cardinal directions, they should not look stretched when you look close, and you should be able to glide smoothly diagonally along flat walls.

## Verification
- **Does the "N" texture look backwards?** Your X-axis mapping for the North face is inverted. Find the condition for `side == 1 && rayDirY < 0` and flip `texX = textureWidth - texX - 1`.
- **Do the textures jitter or flash erratically when you move?** You are likely converting `wallX` from `double` to `int` prematurely before multiplying by texture width.
- **Do the textures "stretch" infinitely at the edges?** Your `drawStart` and `drawEnd` lines might be drawing pixels, but your `texY` calculation might not account for walls taller than `SCREEN_H`.

Passing this generic textured engine capstone implies that the "meat" of the project is complete.
