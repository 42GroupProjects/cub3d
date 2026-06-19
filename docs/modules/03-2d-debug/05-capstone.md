# Capstone 03: The 2D Grid Walker

## The Objective
Before writing any complex 3D math, you must prove that your 2D mathematical foundation is absolutely solid. In this Capstone, you will link your Parser map to your graphics Engine, resulting in a top-down `so_long`-style simulation using float coordinates.

## Requirements Checklist
1. [ ] Create a `t_img` buffer using `mlx_new_image`. 
2. [ ] Write a safe `put_pixel(img, x, y, color)` function.
3. [ ] Iterate over your parsed `char **map` array and use `draw_rect` to paint grey walls and black floor tiles to your `t_img`.
4. [ ] Translate the player's initial `char` position from the grid into initial floating-point `pos_x` and `pos_y` coordinates (e.g. `2.5, 4.5`).
5. [ ] Initialize the `dir_x/y` and `plane_x/y` vectors based on the spawn letter exactly as shown in Chapter `02-player-vector`.
6. [ ] Draw the player as a Red dot on the 2D canvas, and draw a Blue line extending from them using Bresenham/DDA line drawing.
7. [ ] Implement asynchronous keyboard hooks (`w/a/s/d` and `<-/->`).
8. [ ] **Execution:** You should be able to hold `W` and watch the red dot move smoothly along the path of the blue line. You should be able to hold `Left` and watch the blue line rotate perfectly in a circle around the red dot!

## Verification
Do not implement wall collisions yet! Just prove you can rotate the blue line and walk smoothly along whatever vector the line is pointing at. 

Once your Red dot moves and your Blue line rotates without freezing or jumping, you have officially laid the geometric foundation required to start computing 3D space. You have passed the Capstone.
