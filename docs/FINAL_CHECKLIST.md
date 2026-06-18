# Evaluation Checklist

This checklist must pass 100% locally before signing up for evaluation.
Run every test in `tests/valid/` and `tests/invalid/` before ticking any box.

---

## Phase 1 — Argument and File Handling

- [ ] `./cub3d` (no args) prints `Error\n...` and exits non-zero
- [ ] `./cub3d too many args here` prints `Error\n...`
- [ ] `./cub3d non_existent_file.cub` prints `Error\n...` without crashing
- [ ] `./cub3d tests/valid/01_minimal_N.cub` opens a window successfully
- [ ] `.ber` extension file is rejected (wrong extension for cub3d)

---

## Phase 2 — Parser: Valid Maps (must all PASS)

Run each file in `maps/m_valid/` and confirm the window opens and the player starts in the correct orientation.

- [ ] `01_minimal_N.cub` — player faces North on startup
- [ ] `02_large_S.cub` — player faces South on startup
- [ ] `03_small_E.cub` — player faces East on startup
- [ ] `04_facing_W.cub` — player faces West on startup
- [ ] `05_nonrect_L_shape.cub` — non-rectangular map loads, no segfault
- [ ] `06_concentric_maze.cub` — concentric rooms load correctly
- [ ] `07_internal_columns.cub` — internal wall columns render correctly

---

## Phase 3 — Parser: Invalid Maps (must all REJECT with `Error\n`)

Run each file in `maps/m_invalid/` and confirm it prints `Error\n` (and a message) without crashing or opening a window.

- [ ] `01_no_spawn.cub` — rejected: no spawn point
- [ ] `02_double_spawn.cub` — rejected: two spawn points
- [ ] `03_open_right_wall.cub` — rejected: right wall has a gap
- [ ] `04_open_left_wall.cub` — rejected: left wall gap on row 3
- [ ] `05_missing_ceiling_color.cub` — rejected: C line is absent
- [ ] `06_missing_EA_texture.cub` — rejected: EA line is absent
- [ ] `07_duplicate_NO_texture.cub` — rejected: NO declared twice
- [ ] `08_rgb_out_of_range.cub` — rejected: R value is 256
- [ ] `09_rgb_non_numeric.cub` — rejected: color component is `abc`
- [ ] `10_interior_void_gap.cub` — rejected: floor tile touches void/space
- [ ] `11_two_map_blocks.cub` — rejected: two map sections separated by blank line
- [ ] `12_unknown_map_char.cub` — rejected: `X` is not a valid map character
- [ ] `13_wrong_header_order.cub` — rejected OR accepted per subject; check your subject rules
- [ ] `14_no_map_body.cub` — rejected: valid header but no map follows
- [ ] `15_spawn_on_border.cub` — rejected: spawn placed on the border wall row

