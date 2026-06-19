# 03 — Error Robustness

Before you freeze the mandatory part of the project, you must throw garbage at it. The 42 Evaluator will spend 15 minutes explicitly trying to crash your program using malformed `.cub` configs.

## The Checklist of Doom

Create text files testing every single one of these scenarios to ensure your `Error\n` message prints cleanly without segfaulting.

### 1. File Access Edge Cases
- Pass a directory instead of a file: `./cub3d /tmp` -> Should fail cleanly.
- Pass a file without `.cub` extension: `./cub3d map.txt` -> Should fail.
- Pass a file that doesn't exist: `./cub3d fake.cub` -> Check `open()` return val!
- Give a file without read permissions: `chmod 000 map.cub` -> Must handle gracefully.

### 2. Configuration Bloat
- Missing North texture: `NO` is nowhere in the file.
- Re-declared West texture: `WE file.xpm` appears twice.
- Invalid RGB Colors: `F 255,255,-1` or `C 12, 14, 15, 10` or `F 255,,255`.
- Path does not exist: `NO ./does_not_exist.xpm` -> `mlx_xpm_file_to_image` will return NULL. Check it!

### 3. Map Validation Breakage
- **No Player**: Map consists only of `1` and `0`.
- **Multiple Players**: Map contains `N` and `S`.
- **Invalid Characters**: Map contains a `T` hidden in the walls.
- **Unclosed Maps**: A `0` touches the absolute edge of the text file.
- **Space Leakage**: A ` ` (space) touches a `0` or the Player without a wall in between.
- **Empty Map**: The `.cub` file has the textures, but no map array at the bottom.

If your program can ingest all of these and successfully trigger your `clean_exit()` Garbage Collector, your project is officially airtight.
