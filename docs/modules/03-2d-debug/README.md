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

- [ ] Map cells draw at correct positions using `cell_dim`, `offset_x`, `offset_y`
- [ ] Walls darker, floors lighter, void black — correct color scheme
- [ ] Player dot appears at the exact spawn cell on first frame
- [ ] Direction arrow points in the correct direction for N/S/E/W spawns
- [ ] WASD movement works — float position, collision stops at `'1'` cells
- [ ] Left/right rotation rotates the arrow — plane stays perpendicular
- [ ] After 360° rotation, arrow returns to original direction (no drift)
- [ ] *(Stretch)* Yellow center debug ray stops at the correct wall cell

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
