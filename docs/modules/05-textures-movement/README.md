# Module 05 — Textures and Movement

**Branch:** `main` | **Days:** 8–10

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | Overview + checkpoint + crisis cards |
| [01-texture-mapping.md](./01-texture-mapping.md) | Texture loading, wall_x, texX, texY stepping + The Stretching Seizure |
| [02-movement-and-collision.md](./02-movement-and-collision.md) | WASD physics, rotation, sliding collision |

**Code references:**
- `code_snippets/raycasting/derfleck_textures.c` — full texture loading + wall drawing
- `code_snippets/movement/derfleck_move.c` — walk, strafe, rotate with corner check

---

## Exit Checkpoint

- [ ] All 4 textures load — NULL return handled with `Error\n` + exit — XPMs ready in `textures/`; load not implemented
- [ ] North texture appears on North-facing walls (not South, not East)
- [ ] South/East/West textures on correct respective walls — verified on all 4 movement directions
- [ ] No texture stretching at close range
- [x] Floor fills with correct parsed RGB color
- [x] Ceiling fills with correct parsed RGB color
- [x] WASD moves in correct directions
- [x] Player cannot walk through walls (wall-stop at minimum) — basic `'1'` stop only
- [ ] Left/right arrows rotate view correctly — currently Q/E; arrows strafe
- [ ] ESC and window X button exit cleanly — textures freed before destroy

---

## Crisis Cards (Module 05)

| ⚠️ Symptom | Cause | Fix |
|-----------|-------|-----|
| **Wrong texture on wall** — N texture on S wall | `step_x`/`step_y` sign check reversed in texture selection | Check: X-wall + ray going left (`raydir[X] < 0`) = WEST texture |
| **Texture flipped horizontally** | Mirror fix missing | Add: `if (side == 0 && raydir[X] > 0) tex_x = tex_w - tex_x - 1` |
| **The Stretching Seizure** — texture stretches horribly close up | `tex_pos` and `step` are floats but `tex_y` uses integer cast incorrectly | See 01-texture-mapping.md |
| **Floor is wrong color** | Byte order wrong in `color_from_rgb` | Check endianness: `(r << 16) | (g << 8) | b` |
| Segfault on program close | You freed `tex.pixels` — that pointer belongs to MLX | Only destroy `tex.img` with `mlx_destroy_image` |
| Texture only on some walls | `mlx_xpm_file_to_image` returned NULL silently | Add NULL check for each texture after loading |

---

## Peer Explanation Drill

1. What is `wall_x` and what range does it span across one wall cell?
2. Why do we need to mirror `tex_x` depending on ray direction?
3. Explain `tex_step` in words — what does it represent physically?
4. Why do we check X and Y movement separately in the collision step?
5. Why is floor/ceiling just a fill loop and not a per-pixel raycasting operation?
