# Evaluation Checklist

This checklist must pass 100% locally before signing up for evaluation.
Parser map suites live under `maps/m_valid/` and `maps/m_invalid/`.

Last reviewed: 2026-07-21

---

## Phase 1 — Argument and File Handling

- [x] `./cub3d` (no args) prints `Error\n...` and exits non-zero
- [x] `./cub3d too many args here` prints `Error\n...`
- [x] `./cub3d non_existent_file.cub` prints `Error\n...` without crashing
- [x] `./cub3d maps/m_valid/01_minimal_N.cub` opens a window (flat-color walls; textures not sampled yet)
- [x] `.ber` / wrong extension is rejected

---

## Phase 2 — Parser: Valid Maps (must all PASS)

Verified with `make test-maps` / `CUB3D_PARSE_ONLY=1`. Window opens for valid maps.

- [x] `01_minimal_N.cub` — parses; spawn N
- [x] `02_large_S.cub` — parses; spawn S
- [x] `03_small_E.cub` — parses; spawn E
- [x] `04_facing_W.cub` — parses; spawn W
- [x] `05_nonrect_L_shape.cub` — parses (flood-fill enclosure)
- [x] `06_concentric_maze.cub` — parses
- [x] `07_internal_columns.cub` — parses
- [x] `08_big_map.cub` — parses

---

## Phase 3 — Parser: Invalid Maps (must all REJECT with `Error\n`)

Verified via `make test-maps` (+ CI / Vesta invalid suite).

- [x] `01_no_spawn.cub`
- [x] `02_double_spawn.cub`
- [x] `03_open_right_wall.cub`
- [x] `04_open_left_wall.cub`
- [x] `05_missing_ceiling_color.cub`
- [x] `06_missing_EA_texture.cub`
- [x] `07_duplicate_NO_texture.cub`
- [x] `08_rgb_out_of_range.cub`
- [x] `09_rgb_non_numeric.cub`
- [x] `10_interior_void_gap.cub`
- [x] `11_two_map_blocks.cub`
- [x] `12_unknown_map_char.cub`
- [x] `13_wrong_header_order.cub` — in `maps/m_optional/` (accept OK for us)
- [x] `14_no_map_body.cub`
- [x] `15_spawn_on_border.cub`

---

## Still open for mandatory

### Friend (game / learning)

- [ ] Wall textures loaded and sampled (N/S/E/W distinct) — XPMs in `textures/` ready
- [ ] ←/→ rotate look (WASD move/strafe only; Q/E currently rotate)
- [ ] Fix `camera_y` → `camera_x` in `ray.c`
- [ ] Collision / 10×10 footprint (his plan) + OOB/`' '` walk guards
- [ ] Norminette clean on `src/game/`
- [ ] Drop debug `printf`s

### Shared / you can pick up (safe help)

- [x] ESC / window X → `clean_exit` (MLX + config) — extend when texture *images* exist
- [x] Includes headers norm-clean
- [ ] Optional parser harden: trim spaces in RGB parts (`F 1, 2, 3` currently fails)
- [ ] Optional: move/rename `rgb_to_int` helper (game still has `rgbToInt`)
- [ ] Valgrind full play session after textures (use `docs/modules/12-debugging/02-valgrind-cheat-sheet.md`)
- [ ] Oral defense prep (parser half you; DDA/textures him)
- [x] README AI section if campus requires it
