# Cub3D Course Documentation

*Module-based learning system for the 42 cub3D project.*

---

## How This Folder Is Organized

```
docs/
  modules/
    00-subject/          ← Start here — prerequisites, timeline, rules
    01-foundation/       ← MLX engine, image buffer, vectors, 2D canvas
    02-parser/           ← .cub parser, state machine, validation
    03-2d-debug/         ← 2D top-down debug world
    04-raycaster/        ← DDA algorithm, fisheye fix
    05-textures-movement/ ← Textures, floor/ceil, WASD, collision
    06-mandatory-freeze/ ← Norm, leaks, README, oral defense
    07-bonus/            ← Minimap, doors, mouse, sprites (bonus branch)
    08-sprites/          ← Sprite projection, Z-buffer, duck entity (combat-lab)
    09-combat-lab/       ← Weapon, parry, hit detection (combat-lab)
    10-asset-pipeline/   ← Blender → sprite sheet → XPM (combat-lab)
    11-evaluation/       ← Defense drills, templates, question bank
  reference/             ← Formulas, glossary, crisis cards, methodology / oral defense
  journal/               ← Daily, pair review, AI usage, retrospective
```

**Oral defense (start here):** [`reference/methodology-oral-defense.md`](./reference/methodology-oral-defense.md) — our pipeline, ownership, Q&A, drills.

---

## Learning Rule

> **Do not proceed to the next module until the current module's Exit Checkpoint is complete.**

---

## Module Overview Table

| Module | Branch | Days | Opens Window? |
|--------|--------|------|--------------|
| 00-subject | All | Before Day 1 | — |
| 01-foundation | main | 1–2 | Yes |
| 02-parser | main | 3–4 | No (prints parsed config) |
| 03-2d-debug | main | 5 | Yes (2D canvas) |
| 04-raycaster | main | 6–7 | Yes (3D walls) |
| 05-textures-movement | main | 8–10 | Yes (full mandatory) |
| 06-mandatory-freeze | main | 11–12 | — |
| 07-bonus | bonus | 13–15 | Yes (minimap overlay) |
| 08-sprites | combat-lab | After eval | Yes (duck entity) |
| 09-combat-lab | combat-lab | After eval | Yes (weapon overlay) |
| 10-asset-pipeline | combat-lab | Parallel | No (offline Blender) |
| 11-evaluation | All | Ongoing | — |

---

