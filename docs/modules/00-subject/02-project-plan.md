# 00-02 — Project Plan & Timeline

Use this table to drive your sessions. Do not proceed to the next row until the ✅ checkpoint passes.

---

## Intensive 12-Day Timeline

| Day | Module | Goal | ✅ Checkpoint |
|-----|--------|------|---------------|
| **1 AM** | 01-foundation | Window opens, ESC + X button close cleanly | Black window with no crash |
| **1 PM** | 01-foundation | Image buffer + colored rectangle renders | Red square visible on screen |
| **2 AM** | 01-foundation | Direction vector visualized — arrow rotates with left/right | Line segment changes direction |
| **2 PM** | 01-foundation | 2D grid canvas — parsed map drawn with player dot | Player spawns at correct cell |
| **3** | 02-parser | `.cub` header fully parsed + RGB validated | Print parsed config without crash |
| **4** | 02-parser | Map grid extracted, padded, spawn found | `print_map()` shows correct grid |
| **4 PM** | 02-parser | All 15 invalid maps rejected with `Error\n` | Every `tests/mandatory_invalid/` returns nonzero |
| **5** | 03-2d-debug | 2D debug view + single center ray hits wall | Yellow ray stops at wall cell |
| **6** | 04-raycaster | Flat-color walls render with DDA | Walls appear with perspective |
| **7** | 04-raycaster | X/Y shading, fisheye free | Long corridor looks straight |
| **8** | 05-textures | Textures appear on correct walls | North texture on north walls |
| **9** | 05-textures | Floor/ceiling solid color + WASD movement | Full mandatory scene playable |
| **10** | 05-textures | Movement collision (wall stop at minimum) | Cannot walk through walls |
| **11** | 06-freeze | Norminette clean, leaks clean, null guards | `norminette` 0 errors |
| **12** | 06-freeze | README final, both partners pass oral drill | Both can explain DDA without reading code |

---

## Gamified Progress Tracker

Copy this block into your journal when you start each day. Check off items as you go.

```

[x] Window opens                    (Day 1 AM)
[x] Buffer renders a pixel          (Day 1 PM)
[x] Direction / FOV rotates         (Day 2 AM)  — 3D rotate works (←/→ still wrong keybind)
[ ] 2D grid canvas done             (Day 2 PM)  — draw_map exists but commented out of render
[x] Header parsed correctly         (Day 3)
[x] Map extracted + padded          (Day 4 AM)
[x] All 15 invalid maps rejected    (Day 4 PM)  — 14 in m_invalid + suites; #13 optional
[ ] Center debug ray visible        (Day 5)     — skipped / commented
[x] Flat-color walls render         (Day 6)
[x] No fisheye on long corridor     (Day 7)     — perp_wall_dist used; camera_y bug still open
[ ] Textured walls correct          (Day 8)     — XPMs on disk; game does not load them yet
[x] Floor + ceiling + WASD          (Day 9)     — F/C + WASD ok; arrows must rotate (not strafe)
[x] No wall clipping (basic stop)   (Day 10)    — stops on '1'; OOB/' ' still weak
[ ] Norminette clean                (Day 11)    — parser/main OK; game + headers not
[ ] Both pass oral defense drill    (Day 12)

```

---

## Checkpoint Logic

> **"Do not proceed to textures until a red square moves on screen with the game loop."**

The purpose of the checkpoint column is to give you a concrete, observable proof that the current module is working before complexity is added on top of a broken foundation.

Common bad pattern: student skips 2D debug view and goes straight to 3D. When the 3D raycaster produces wrong output (mirrored walls, wrong textures, fisheye) they have no way to verify the underlying vectors and grid logic. The 2D canvas prevents this.

---

## Pair Work Rules

- **Alternate driver/reviewer every 2 hours**
- **Both must understand and be able to explain every function**
- Both must individually answer the peer explanation drill at the end of each module
- Track driver/reviewer in `docs/journal/pair-review-template.md`

---

## After Day 12: Bonus Path

| Day | Module | Goal |
|-----|--------|------|
| 13 | 07-bonus | Minimap rendered in corner |
| 13 PM | 07-bonus | Mouse rotation working |
| 14 | 07-bonus | Wall sliding collision |
| 14 PM | 07-bonus | Doors toggle on proximity |
| 15 | 07-bonus | Animated sprites with Z-buffer |
| 16+ | 08–10 | Combat-lab: duck entity, sword, parry |
