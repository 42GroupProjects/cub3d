# Module 08 — Sprite Entities and Duck

**Branch:** `combat-lab` | **After evaluation**

> Inspired by [this Dylearn video](https://www.youtube.com/watch?v=py0U3Bq8azI). No `combat-lab` work begins until `main` is evaluated.

The duck is **not** a 3D model at runtime. It is a **sprite entity** — a billboard quad in the engine, textured with pre-rendered sprite sheet frames from fixed angles.

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | Overview + checkpoint |
| [01-sprite-projection.md](./01-sprite-projection.md) | Sprite math: world-to-screen projection + Z-buffer |
| [02-entity-state-machine.md](./02-entity-state-machine.md) | States: IDLE / WALK / HIT / DEAD + animation |
| [03-z-buffer.md](./03-z-buffer.md) | How the depth buffer prevents sprites drawing through walls |

---

## Core Concept: Sprite Projection Math

For each entity with world position `(entity_x, entity_y)`:

```c
// 1. Vector from player to sprite
double sprite_x = entity->x - pos_x;
double sprite_y = entity->y - pos_y;

// 2. Inverse camera matrix transform
// (undoes the camera rotation to get screen-space coordinates)
double inv_det = 1.0 / (plane_x * dir_y - dir_x * plane_y);
double transform_x = inv_det * ( dir_y   * sprite_x - dir_x   * sprite_y);
double transform_y = inv_det * (-plane_y * sprite_x + plane_x * sprite_y);
//                   ^
//                   transform_y is the depth — positive = in front of player

// 3. Screen X center of the sprite
int sprite_screen_x = (int)((SCREEN_W / 2) * (1 + transform_x / transform_y));
```

> `transform_y` is the corrected perpendicular depth. If `transform_y <= 0`, the sprite is behind the player — skip drawing it. The sprite is only visible when `transform_y > 0`.

---

## Z-Buffer Integration

During the raycasting loop (Module 04), store `perpWallDist` per screen column:

```c
double z_buffer[SCREEN_W];    // one entry per screen column

// At the end of each column's DDA:
z_buffer[x] = perp_wall_dist;
```

When drawing sprites, only draw pixels where `transform_y < z_buffer[x]` (sprite is in front of wall):

```c
for (int stripe = draw_start_x; stripe < draw_end_x; stripe++)
{
    if (transform_y > 0 && stripe > 0 && stripe < SCREEN_W
    && transform_y < z_buffer[stripe])
    {
        int tex_x = (int)((stripe - (-sprite_width / 2 + sprite_screen_x))
                       * tex_width / sprite_width);
        // draw sprite column at stripe
    }
}
```

---

## Entity State Machine

```
IDLE
  → player enters detection range
  → WALK (move toward player)
    → player within melee range
    → ATTACK_WINDUP
      → attack window opens
      → ATTACKING
        → player parries: entity → STUNNED
        → player doesn't parry: player takes damage, entity → IDLE
    → player leaves range: IDLE

HIT    (takes damage)
  → damage applied
  → HURT_ANIMATION (few frames)
  → if HP <= 0: DEAD
  → else: IDLE

DEAD
  → death animation plays
  → entity removed from list
```

---

## Suggested Map Symbols (combat-lab only)

| Symbol | Meaning |
|--------|---------|
| `K` | Duck entity spawn point |
| `T` | Tree sprite (static, no state machine) |
| `G` | Grass cluster |

---

## Exit Checkpoint

- [ ] Duck sprite renders at correct screen position
- [ ] Duck appears in front of walls when closer (Z-buffer correct)
- [ ] Duck behind walls when farther (not drawn through walls)
- [ ] Duck's facing angle changes which sprite-sheet direction shows
- [ ] Basic idle animation cycles (frame counter increments with dt)
