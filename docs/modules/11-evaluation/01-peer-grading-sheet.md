# 01 — Simulated Peer Grading Sheet

Before you sign up for your evaluation, grab a friend and pretend they are a strict evaluator. Run through this checklist over Discord.

## 1. Safety Checks (Must pass or FAIL immediately)
- Run `norminette` explicitly on your project folder. Ensure `0 OK`.
- Run `make clean` then `make`. Ensure the binary is named exactly `cub3D`.
- Check there are zero relinks (`make` followed by `make` must say "Nothing to be done").
- Is there any `malloc` or `free` happening inside the main `mlx_loop_hook` rendering loop? (Memory leak danger!).

## 2. Invalid Input Tests (Evaluator tries to crash you)
Command line attacks:
- `./cub3D` (No arguments)
- `./cub3D map.cub map2.cub` (Too many arguments)
- `./cub3D fakefile.cub`
- `./cub3D Makefile`

Config file attacks:
- Create a `.cub` mapping `NO` texture to `/dev/null` or `/dev/random`.
- Create a `.cub` with `F 255.0 , 255 , 255`.
- Create a `.cub` where a wall line ends in `11101` and the row below it has a `0` underneath that space.

## 3. Playtesting (Window and Physics)
- Rapidly tap `WASD` while slamming your mouse left and right. Does the window crash?
- Walk directly into a flat wall and hold `W`. Turn right while holding `W`. Do you slide smoothly along the wall, or do you get completely stuck?
- Press `ESC`. Open the game again. Press the Red `X` on the macOS window bar. Does it close properly both times?

## 4. The Leaks Final Drill
- Start the game (`./cub3d maps/maze.cub`).
- Run `leaks cub3D` in another terminal. Look carefully. Are there thousands of bytes leaking?
- Force an error: `./cub3d invalid_map.cub`. Run `valgrind` or check your C memory tracker. Are you freeing the map array before exiting?

If you can pass this simulated sheet without defending or arguing ("Oh it crashes but only if you..."), you are ready to evaluate.
