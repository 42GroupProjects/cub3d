# Capstone 06: The Final Mandatory Push

## The Objective
This is the most critical milestone in your Cub3D journey. The "Bonus" mechanics (sprites, doors, minimap) are **worthless** if your mandatory project fails any checklist item during the defense. 

Your objective is to finalize the mandatory core, pass the strict Norminette rules, verify 0 memory leaks, and push an immaculate version to your Git `main` branch.

## Requirements Checklist
1. [ ] **Norm Compliance**: Run `norminette src/ includes/ libft/`. If it returns a single red error, you cannot push. Re-architect functions to meet the 25-line rule.
2. [ ] **Double Buffering Verified**: Ensure `mlx_pixel_put` does not exist anywhere in your codebase.
3. [ ] **Fisheye Fixed**: Verify `perpWallDist` is used precisely and walls look flat when panning the camera.
4. [ ] **Texture Wrapping Fixed**: Verify that the North, South, East, and West textures display perfectly without mirroring backwards.
5. [ ] **Robustness Script Passed**: Run your engine against 10 invalid `.cub` maps. It must print `Error` and exit without Segfaulting.
6. [ ] **Valgrind / Leaks Zeroed**: Verify `leaks -atExit -- ./cub3d map.cub` returns exactly 0 leaks. Close the window with ESC. Close it with the Red X button. Verify both exit paths free everything.
7. [ ] **Bonus Isolation**: Ensure your `main` branch contains NO MINIMAP, NO MOUSE ROTATION, and NO SPRITES. The 42 subject explicitly states that the mandatory part must be perfect in its own strict definition.

## Verification
If you can check all 7 boxes, your engine is legally ready to pass the 100/100 threshold. 
You are now officially cleared to `git checkout -b bonus` and begin hacking in the fun stuff. You have passed the Capstone.
