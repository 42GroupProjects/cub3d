# Capstone 04: The Wolfenstein 3D Core

## The Objective
We are leaving the 2D Top-Down canvas behind. The goal of this Capstone is to bind your DDA raycaster math into the rendering engine. The result should look exactly like early 1992 Wolfenstein 3D: flat colored, shaded walls that you can walk between without crashing.

## Requirements Checklist
1. [ ] Strip out your 2D grid rendering loop. Replace it with a `while (x < SCREEN_WIDTH)` DDA raycasting loop.
2. [ ] For every X column, generate `rayDirX` and `rayDirY`.
3. [ ] Calculate the `deltaDist` and initial `sideDist` blocks.
4. [ ] Run the `while(hit == 0)` DDA collision loop to find the nearest `map[][] == '1'`.
5. [ ] Calculate the `perpWallDist` to mathematically correct fisheye distortion.
6. [ ] Calculate `drawStart` and `drawEnd`.
7. [ ] Pick a solid color (e.g. Blue). If the `side == 1`, divide the color by 2 to shadow it.
8. [ ] **Execution:** You should now be inside a blue 3D maze. As you hold WASD and the Arrow Keys, you should navigate it flawlessly in first-person format.

## Verification
- **Does it crash when walking into corners?** If yes, your `sideDist` or grid checking logic failed. Add padding logic to your keyboard inputs.
- **Do the walls look horribly curved like a barrel?** You are using standard Euclidean distance. Switch to the `perpWallDist` formula.
- **Is the application freezing randomly?** Make sure you initialized `hit = 0` at the start of every column loop. Do not leave it set to `1` from the previous pixel column!

Once you can walk around your map in 3D without a crash, you have a fully functional raycaster. You have passed the Capstone.
