# Evaluation Checklist

This checklist must pass 100% locally before signing up for evaluation.
Parser map suites live under `maps/m_valid/` and `maps/m_invalid/`.

---

## Phase 1 — Argument and File Handling

- [x] `./cub3d` (no args) prints `Error\n...` and exits non-zero
- [x] `./cub3d too many args here` prints `Error\n...`
- [x] `./cub3d non_existent_file.cub` prints `Error\n...` without crashing
- [ ] `./cub3d maps/m_valid/01_minimal_N.cub` opens a window successfully — window opens; textures not drawn yet
- [x] `.ber` / wrong extension is rejected

---

## Phase 2 — Parser: Valid Maps (must all PASS)

Parse path verified with `make test-maps` / `CUB3D_PARSE_ONLY=1`. Window + orientation still need a quick manual check once textures work.

- [x] `01_minimal_N.cub` — parses; spawn N on map
- [x] `02_large_S.cub` — parses; spawn S
- [x] `03_small_E.cub` — parses; spawn E
- [x] `04_facing_W.cub` — parses; spawn W
- [x] `05_nonrect_L_shape.cub` — parses (flood-fill enclosure)
- [x] `06_concentric_maze.cub` — parses
- [x] `07_internal_columns.cub` — parses
- [x] `08_big_map.cub` — parses (extra stress map)

Manual (after game textures): confirm window + facing for each of the above.

---

## Phase 3 — Parser: Invalid Maps (must all REJECT with `Error\n`)

Verified via `make test-maps` (and CI / Vesta invalid suite).

- [x] `01_no_spawn.cub` — rejected: no spawn point
- [x] `02_double_spawn.cub` — rejected: two spawn points
- [x] `03_open_right_wall.cub` — rejected: open right
- [x] `04_open_left_wall.cub` — rejected: open left
- [x] `05_missing_ceiling_color.cub` — rejected: missing C
- [x] `06_missing_EA_texture.cub` — rejected: missing EA
- [x] `07_duplicate_NO_texture.cub` — rejected: duplicate NO
- [x] `08_rgb_out_of_range.cub` — rejected: RGB out of range
- [x] `09_rgb_non_numeric.cub` — rejected: non-numeric RGB
- [x] `10_interior_void_gap.cub` — rejected: floor touches void
- [x] `11_two_map_blocks.cub` — rejected: split map / empty line case
- [x] `12_unknown_map_char.cub` — rejected: unknown char
- [x] `13_wrong_header_order.cub` — moved to `maps/m_optional/` (subject: accept or reject)
- [x] `14_no_map_body.cub` — rejected: no map
- [x] `15_spawn_on_border.cub` — rejected: spawn on border

---

## Still open for mandatory (game / freeze)

- [ ] Wall textures loaded and sampled (N/S/E/W distinct)
- [ ] ←/→ rotate look (WASD move/strafe only)
- [x] ESC / window X call `clean_exit` (frees MLX + parsed config) — extend when texture images exist
- [ ] Norminette clean on `src/game/` + remaining header nits
- [ ] Full oral defense (both partners: DDA + textures + cleanup)
