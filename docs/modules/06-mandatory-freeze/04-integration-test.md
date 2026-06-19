# 04 — Pre-Submission Integration Test

Before touching a single line of bonus code, you must freeze the mandatory submission in a passing state. This file defines the **complete integration test battery** — the exact tests a 42 evaluator runs in the first 20 minutes.

If any test in this file fails, fix it before touching bonus code. Bonus points cannot compensate for a broken mandatory.

---

## Why This Phase Exists

The most dangerous trap in cub3D: starting bonus work before the mandatory is watertight. Students add minimap code that inadvertently breaks the parser, or mouse-look that corrupts the camera plane, and they cannot isolate which commit introduced the regression.

**Rule:** Tag your git repository with `v1.0-mandatory` before writing a single bonus line.

```bash
git add -A
git commit -m "feat: mandatory engine complete and passing"
git tag v1.0-mandatory
```

If bonus work breaks something, `git diff v1.0-mandatory` shows exactly what changed.

---

## Test Battery: File System & Arguments

Run each command. Confirm the stated behavior exactly.

```bash
# T-01: No arguments
./cub3d
# Expected: "Error\n<usage message>" on stderr, exit code != 0, no crash

# T-02: Too many arguments
./cub3d map.cub extra_arg
# Expected: "Error\n<usage message>" on stderr

# T-03: Wrong extension
./cub3d map.txt
./cub3d map.cubb
./cub3d map
# Expected: "Error\n<extension message>" on stderr

# T-04: Non-existent file
./cub3d does_not_exist.cub
# Expected: "Error\n<not found message>", NOT a segfault

# T-05: Directory instead of file
./cub3d /tmp
./cub3d ./maps/
# Expected: "Error\n<is a directory message>", NOT a silent hang

# T-06: No read permission
chmod 000 tests/perms.cub
./cub3d tests/perms.cub
chmod 644 tests/perms.cub
# Expected: "Error\n<permission denied message>"
```

---

## Test Battery: Malformed Config Headers

```bash
# T-07: Empty file
touch tests/empty.cub && ./cub3d tests/empty.cub
# Expected: "Error\nMissing required headers"

# T-08: Missing one header (NO missing)
./cub3d tests/missing_no.cub
# Expected: "Error\n" without crash

# T-09: Duplicate header
./cub3d tests/dup_no.cub    # File with NO listed twice
# Expected: "Error\nDuplicate identifier"

# T-10: Header with no value
./cub3d tests/no_empty.cub  # File with "NO" and nothing after
# Expected: "Error\n" — NOT a segfault dereferencing parts[1]

# T-11: Invalid texture path (file does not exist)
./cub3d tests/bad_tex_path.cub
# Expected: "Error\nCannot load texture" — NOT a crash inside MLX

# T-12: RGB out of range
./cub3d tests/rgb_256.cub      # F 256,0,0
./cub3d tests/rgb_negative.cub # F -1,0,0
./cub3d tests/rgb_alpha.cub    # F abc,0,0
./cub3d tests/rgb_comma.cub    # F 255,,255
# Each: "Error\n<rgb message>", no crash

# T-13: Map before all headers complete
./cub3d tests/map_before_headers.cub
# Expected: "Error\n"
```

---

## Test Battery: Malformed Maps

```bash
# T-14: No player spawn
./cub3d tests/no_spawn.cub
# Expected: "Error\nNo player spawn"

# T-15: Multiple player spawns
./cub3d tests/multi_spawn.cub
# Expected: "Error\nMultiple spawns"

# T-16: Invalid character in map
./cub3d tests/invalid_char.cub   # Map contains '2' or '@'
# Expected: "Error\nInvalid character"

# T-17: Open wall (right edge leak)
./cub3d tests/open_right.cub
# Expected: "Error\nMap not enclosed"

# T-18: Space leaking to floor tile
./cub3d tests/space_leak.cub     # ' ' adjacent to '0' outside walls
# Expected: "Error\nMap not enclosed"

# T-19: Empty map section (headers only, no grid)
./cub3d tests/no_map.cub
# Expected: "Error\nMap section is empty"
```

---

## Test Battery: Valid Maps (Must Open and Run)

```bash
# T-20: Minimal valid map — North-facing spawn
./cub3d tests/valid_minimal_N.cub
# Expected: Window opens, player faces North, WASD + arrows work, ESC closes cleanly

# T-21: South-facing spawn
./cub3d tests/valid_minimal_S.cub

# T-22: East-facing spawn
./cub3d tests/valid_minimal_E.cub

# T-23: West-facing spawn
./cub3d tests/valid_minimal_W.cub
# All: Window opens, correct initial view direction, clean exit

# T-24: Map with interior void spaces
./cub3d tests/valid_irregular.cub
# Expected: Complex shaped map, map with interior spaces runs without OOB access

# T-25: Large map (24x24 walled room)
./cub3d tests/valid_large.cub
# Expected: Runs at acceptable FPS, no memory corruption on edge walls

# T-26: Close walls (tight corridor, 3 tiles wide)
./cub3d tests/valid_corridor.cub
# Expected: Very tall wall rendering when close, no clamp overflow crash
```

---

## Memory Leak Verification

Run the Valgrind battery on **every** exit path:

```bash
# Clean exit via ESC key
valgrind --leak-check=full --track-origins=yes \
  ./cub3d tests/valid_minimal_N.cub
# Expected: 0 bytes definitely/indirectly lost

# Window close button (X)
# (open the window, click X immediately)
valgrind --leak-check=full ./cub3d tests/valid_minimal_N.cub

# Error exit path
valgrind --leak-check=full ./cub3d tests/no_spawn.cub
# Expected: 0 bytes definitely lost, even during error path teardown

# macOS alternative (faster feedback)
leaks -atExit -- ./cub3d tests/valid_minimal_N.cub
```

**Target:** `0 bytes in 0 blocks are definitely lost`. Still-reachable MLX internals are acceptable if suppressions confirm they are library-internal.

---

## Norminette Verification

```bash
# Full compliance scan
norminette src/ includes/
# Expected: "OK!" on every file — zero errors

# Common violations to manually check:
# - DDA loop split into sub-functions (each < 25 lines)
# - No for-loops (all converted to while)
# - All variables declared at top of block, not inside if/else
# - No inline comments (all comments are block comments above the function)
# - No global variables (all data lives in t_game)
# - Function parameters <= 4
```

---

## Performance Baseline

With your FPS counter active (added in module 12), record baselines on both machines:

```
Machine      | Map              | FPS  | Frame Time
-------------|------------------|------|----------
Mac (local)  | valid_minimal_N  | ___  | ___ms
Mac (local)  | valid_large      | ___  | ___ms
Ubuntu (42)  | valid_minimal_N  | ___  | ___ms
Ubuntu (42)  | valid_large      | ___  | ___ms
```

**Minimum acceptable:** 30 FPS on both platforms. If below 30 FPS:
1. Confirm `mlx_pixel_put` is NOT in the render loop (switch to buffer)
2. Profile frame phases (see module 12 — DDA vs Sprite vs Flush timing)

---

## Oral Defense Dry Run

Both partners must be able to answer these without notes. Time each other:

| Question | Time Limit |
|----------|-----------|
| Open the binary, describe what happens from `argv[1]` to first rendered pixel | 2 min |
| Point to the exact line where `perpWallDist` is calculated and explain why it cannot be Euclidean distance | 1 min |
| Explain the 4-branch texture selection logic (`side` + `rayDir` sign) | 1 min |
| Show the `clean_exit` function and walk through the teardown order and why it's that order | 1 min |
| Add a LIVE debug print showing the player's `pos_x` and `pos_y` to the terminal every second without breaking the build | 5 min |
| Intentionally break the texture flip logic. Explain what visual bug appears and why | 2 min |

If both partners pass all questions: **mandatory freeze approved.**

---

## The Freeze Commit

```bash
git add -A
git status       # Confirm only your files are staged, no test trash
git commit -m "feat(mandatory): engine complete — all 26 tests passed, 0 leaks, norminette clean"
git tag v1.0-mandatory
git push origin main --tags
```

You may now begin Module 07 (Bonus).
