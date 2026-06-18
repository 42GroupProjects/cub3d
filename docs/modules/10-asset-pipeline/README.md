# Module 10 — Duck Asset Pipeline

**Branch:** `combat-lab` | **Offline work**

This module is offline — Blender does the rigging and rendering. cub3D only plays back the final frames.

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | Workflow overview |
| [01-blender-setup.md](./01-blender-setup.md) | Rigging, animation actions, render setup |
| [02-sprite-sheet-export.md](./02-sprite-sheet-export.md) | Batch render + PNG assembly into sprite sheets |
| [03-import-to-cub3d.md](./03-import-to-cub3d.md) | XPM conversion + loading in engine |

---

## Scope Boundary

| Blender's job | cub3D's job |
|--------------|------------|
| Geometry, rigging, animations | PNG loading + frame cycling |
| Rendering sprite sheets from fixed angles | `mlx_xpm_file_to_image` |
| Palette reduction + transparency | Entity state → sprite selection |

---

## Animation Actions

Create these actions in Blender:

| Action | Frames | Loop? |
|--------|--------|-------|
| `idle` | 8–12 | Yes |
| `walk` | 8 | Yes |
| `attack` | 8–12 | No (one-shot) |
| `hurt` | 4 | No |
| `dead` | 8–12 | No (freeze on last frame) |

---

## Render Angles

For 4-direction sprites: render at 0°, 90°, 180°, 270° (North, East, South, West-facing duck).

Use an orthographic camera at fixed height and distance. Automate with a Blender Python script:

```python
import bpy, math

angles  = [0, 90, 180, 270]
actions = ['idle', 'walk', 'attack', 'hurt', 'dead']

for angle in angles:
    bpy.data.objects['Camera'].rotation_euler[2] = math.radians(angle)
    for action in actions:
        bpy.data.objects['Duck'].animation_data.action = bpy.data.actions[action]
        # Set frame range, render output path, call bpy.ops.render.render(animation=True)
```

---

## Export Settings

- Format: PNG, RGBA (transparent background)
- Resolution: 128×128 per frame (render high, scale down at import if needed)
- Naming: `duck_idle_0deg_f001.png`, `duck_idle_0deg_f002.png`, etc.

---

## Sprite Sheet Assembly

Pack frames horizontally with ImageMagick:

```bash
# Assemble idle animation (N-facing) into single sheet
convert duck_idle_0deg_f*.png +append duck_idle_N.png
# Repeat for each action × direction
```

---

## Import Into cub3D (PNG → XPM)

MiniLibX on 42 machines supports XPM. Convert with ImageMagick:

```bash
convert duck_idle_N.png duck_idle_N.xpm
```

Load in engine:

```c
tex.img    = mlx_xpm_file_to_image(game->mlx, "assets/sprites/duck_idle_N.xpm",
                                   &tex.width, &tex.height);
tex.pixels = mlx_get_data_addr(tex.img, &tex.bpp, &tex.line_length, &tex.endian);
```

---

## Asset Naming Convention

```
assets/sprites/duck/
  duck_idle_N.xpm      ← 8–12 frames wide, 128px tall
  duck_idle_S.xpm
  duck_idle_E.xpm
  duck_idle_W.xpm
  duck_walk_N.xpm
  duck_attack_N.xpm
  duck_hurt_N.xpm
  duck_dead_N.xpm
  ...
```

---

## Exit Checkpoint

- [ ] All 5 actions × 4 directions = 20 sprite sheets exported
- [ ] Each sheet loads without error in the engine
- [ ] Duck renders in idle loop in the entity system (Module 08 connected)
- [ ] Correct direction sprite selected based on entity→player angle
