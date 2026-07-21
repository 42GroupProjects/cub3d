# Module 03 — 2D Debug World

**Branch:** `main` | **Day:** 5

The bridge between your parser and your raycaster. Build this before touching 3D.

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | Checkpoint + crisis cards |
| [01-grid-canvas.md](./01-grid-canvas.md) | Drawing the map, player dot, direction arrow, and debug ray |

---

## Why This Exists

When your 3D raycaster produces wrong output (mirrored walls, fisheye, wrong texture side), you need a ground truth. The 2D canvas lets you:
- Verify that `dir` and `plane` are initialized correctly
- Confirm the player spawns at the right map cell
- Cast one ray and watch it hit the correct wall
- Test rotation logic visually before it powers the 3D engine

**Common mistake:** Students skip this and go straight to 3D. When bugs appear, they have no way to distinguish: wrong DDA direction vs wrong `side` detection vs wrong texture selection. The 2D canvas removes this ambiguity completely.

---

## Exit Checkpoint

Status vs current code (2026-07-21). Live `render()` runs **3D only**; 2D helpers exist but are commented out.

- [ ] Map cells draw at correct positions using `cell_dim`, `offset_x`, `offset_y` — **partial:** `draw_map` uses fixed `TS` tiles, no centering/`cell_dim`; **not called** in `render`
- [ ] Walls darker, floors lighter, void black — **partial:** walls white / else black; no distinct void vs floor; **not in live render**
- [ ] Player dot appears at the exact spawn cell on first frame — **partial:** `draw_player` draws a small red square at float `x*TS`; **not in live render**
- [ ] Direction arrow points in the correct direction for N/S/E/W spawns — **missing:** no 2D direction arrow drawn (only `dir`/`plane` in 3D)
- [x] WASD movement works — float position, collision stops at `'1'` cells — works in 3D path (`handle_input` + `is_walkable`)
- [x] Left/right rotation rotates view — plane stays perpendicular — `rotate_player` updates `dir` + `plane` (keys are Q/E today, not ←/→)
- [ ] After 360° rotation, arrow returns to original direction (no drift) — not formally checked; no 2D arrow to verify
- [ ] *(Stretch)* Yellow center debug ray stops at the correct wall cell — `cast_single_ray` is fully commented out

**Bottom line:** Module 03 was skipped as a live debug view. You jumped to 3D. Enough movement/rotation logic exists underneath, but the 2D canvas checkpoint is **not finished** as written.

---

## Crisis Cards (Module 03)

| ⚠️ Symptom | Cause | Fix |
|-----------|-------|-----|
| Grid squished or stretched | Using only width or only height for `cell_dim` | Take `min(canvas_w / map_w, canvas_h / map_h)` |
| Player dot not at spawn | Placing at integer cell, not float center | Place at `(spawn_x + 0.5) * cell_dim + offset_x` |
| Grid not centered on screen | `offset` calculation wrong | `offset_x = (canvas_w - cell_dim * map_w) / 2` |
| Arrow points wrong at N spawn | Y-down: North = `dir_y = -1`, not `+1` | Confirm spawn direction vectors |
| Arrow drifts in length | Forgot to save `old_plane_x` before rotation | Save both old values before computing rotation |
| Debug ray goes through walls | DDA stop condition wrong | Stop when `map[gy][gx] == '1'` |
