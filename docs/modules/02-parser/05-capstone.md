# Capstone 02: Full Context Parser

## The Objective
Before writing a single line of MLX or graphics code, you must build a standalone C program that verifies you can perfectly parse the `.cub` map format. 

You should be able to run `./cub3d valid.cub` and `./cub3d invalid.cub`. If the map is valid, print the parsed `t_game` struct to the terminal. If invalid, cleanly print `Error\n<Reason>` and exit without memory leaks.

## Requirements Checklist
1. [ ] Rejects files that do not end in `.cub`.
2. [ ] Reads all 6 texture/color identifiers in any order.
3. [ ] Verifies texture paths actually exist (can be opened via `open()`).
4. [ ] Converts `F` and `C` comma-strings into valid 32-bit integer colors.
5. [ ] Refuses to parse the map grid if all 6 identifiers haven't been found yet.
6. [ ] Validates the map is completely closed by `1`s, including diagonally around spaces.
7. [ ] Ensures exactly one player start position (`N`, `S`, `E`, `W`) exists.
8. [ ] Exits cleanly with zero Leaks even when parsing fails mid-map.

## Verification
Create an `invalid_maps/` folder containing the following edge cases:
- `missing_floor.cub`
- `double_player.cub`
- `wall_gap.cub`
- `map_starts_too_early.cub`
- `spaces_inside_map.cub` (Should be valid IF surrounded by walls)

Run Valgrind or `leaks --atExit -- ./cub3d invalid_maps/wall_gap.cub`. If the terminal shows `0 bytes leaked`, you have passed the Capstone.
