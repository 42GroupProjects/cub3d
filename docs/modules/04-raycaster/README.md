# Module 04 — Raycaster Core

**Branch:** `main` | **Days:** 6–7

This is the mathematical center of the project. Take your time here. Math errors here compound into every later visual bug.

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | Overview + checkpoint + crisis cards |
| [01-dda-algorithm.md](./01-dda-algorithm.md) | **Full DDA deep dive** with real annotated code + fisheye fix |

**Code references:**
- `code_snippets/raycasting/derfleck_raycast.c` — clean, well-commented DDA implementation

---

## Why DDA?

Naive raycasting casts a ray and steps along it by tiny increments (0.01 units) until it hits a wall. At 800px wide, 60fps, this is catastrophically slow.

**Digital Differential Analysis (DDA)** skips directly to the next grid line. Instead of stepping 0.01 at a time, it asks "how far along the ray until I cross the next X grid line?" then "how far until the next Y grid line?" and takes whichever crossing comes first. This jumps the ray across an entire grid cell in one step.

For a 800×600 window at 30fps, DDA visits at most ~30,000 cells/frame instead of millions of tiny steps.

---

## Exit Checkpoint

- [x] Full-width wall field renders at correct screen columns (flat colors)
- [x] Walls taller when closer, shorter when farther — perspective works
- [ ] Left/right rotation deforms walls correctly — rotate works, but bind ←/→ (not Q/E)
- [x] Forward/back movement changes wall depth correctly
- [x] X-side hits are different shade than Y-side hits (flat color sides)
- [x] Fisheye largely handled via `perp_wall_dist` — recheck after fixing `camera_y` bug
- [ ] No crash from DDA stepping out of map bounds — add bounds checks (see TODO in `ray.c`)

---

## Crisis Cards (Module 04)

| ⚠️ Symptom | Cause | Fix |
|-----------|-------|-----|
| **Fisheye** — walls curve at screen edges | Using Euclidean distance instead of perpendicular distance | Use `perpWallDist = sideDistX - deltaDistX` (not `sqrt(...)`) |
| Walls all the same height / no depth | `perpWallDist` is constant — DDA not accumulating correctly | Re-check `sideDistX += deltaDistX` inside the step loop |
| Mirror effect — right side shows on left | `camera_x` mapping inverted | Confirm formula: `camera_x = 2 * x / (double)W - 1.0` |
| Walls vibrate when rotating | Floating-point drift in `sideDistX` initialization | Carefully re-check step/init: `sideDistX = (1 - frac_x) * deltaDistX` |
| All walls same texture side color | `side` variable not updated inside DDA loop | Confirm `side = 0` on X step, `side = 1` on Y step |
| DDA crashes into `map[-1][x]` | Stepped out of map bounds | Add bounds check inside DDA loop: `if (mapY < 0 || mapY >= height)` |

---

## Peer Explanation Drill

1. What range does `camera_x` span across the screen? What value is at the center column?
2. What is `deltaDistX` and what does it physically represent in world-space?
3. Why does Euclidean distance to a wall cause fisheye?
4. Explain `perpWallDist` in one sentence. What are its units?
5. After the DDA loop exits, what does `side` tell you?
6. Why is checking `side == 0 && rayDirX < 0` necessary for texture selection?

---

## Journal Prompt

- Paste the code of your DDA step loop (just the `while (!hit)` part)
- Annotate each line in comments: what does it do and why
- Describe the exact moment your first flat-color walls appeared
- Which formula took the longest to understand — deltaDistX, sideDistX, or perpWallDist?
