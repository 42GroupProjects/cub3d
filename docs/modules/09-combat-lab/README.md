# Module 09 — Combat Lab: Weapon and Parry

**Branch:** `combat-lab` | **After Module 08**

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | Overview + attack/parry logic |
| [01-weapon-overlay.md](./01-weapon-overlay.md) | First-person weapon sprite, animation states |
| [02-melee-detection.md](./02-melee-detection.md) | Hit detection: distance + angle + wall occlusion |
| [03-parry-timing.md](./03-parry-timing.md) | Parry window, wind-up signal, stagger state |

---

## Inspiration

[Dylearn combat reference](https://www.youtube.com/watch?v=py0U3Bq8azI) — the combat sensation comes from:
1. **A readable wind-up phase** before the enemy attacks (giving you time to parry)
2. **A tight melee hit window** (2–3 frames) that feels satisfying to land
3. **A punishing parry fail state** — damage taken is noticeable

---

## Weapon Overlay

The weapon sprite draws at the bottom-center of the screen buffer, **after** all raycasting is done for the frame. It is a 2D blit — no projection:

```c
// After raycast_frame(game) completes:
draw_weapon_overlay(game);    // blit weapon frame at fixed screen position
```

Weapon frames are driven by `game->attack_state`:

| State | Frames | Behavior |
|-------|--------|---------|
| `WEAPON_IDLE` | 1 | Static weapon, slight bob with dt |
| `WEAPON_WINDUP` | 2–3 | Pre-swing telegraph |
| `WEAPON_ACTIVE` | 1–2 | Hit window open — check for entities in range |
| `WEAPON_RECOVERY` | 3–4 | Post-swing, no new attack possible |

---

## Melee Hit Detection

When `WEAPON_ACTIVE` begins:

```c
void    check_melee_hit(t_game *game)
{
    t_entity    *e;
    double       dx;
    double       dy;
    double       dist;
    double       angle;

    e = game->entities;
    while (e)
    {
        dx   = e->x - game->pos_x;
        dy   = e->y - game->pos_y;
        dist = sqrt(dx * dx + dy * dy);

        if (dist < MELEE_RANGE)
        {
            // Angle between player's look direction and entity direction
            angle = atan2(dy, dx) - atan2(game->dir_y, game->dir_x);

            // Normalize angle to [-PI, PI]
            while (angle >  M_PI) angle -= 2 * M_PI;
            while (angle < -M_PI) angle += 2 * M_PI;

            if (fabs(angle) < MELEE_ANGLE)        // within swing cone
                apply_hit(e, game->weapon_damage); // deal damage
        }
        e = e->next;
    }
}
```

Constants to tune:
- `MELEE_RANGE` — world units (try 1.5)
- `MELEE_ANGLE` — radians (try `M_PI / 4` = 45° cone)
- `weapon_damage` — HP decrease per hit

---

## Parry Logic

The entity state `ATTACK_WINDUP` is the window the player can parry:

```c
/*
 * Parry check: called when player presses parry key.
 * If any entity is in ATTACK_WINDUP state, within range, and in front:
 *   → entity enters STUNNED state (cannot attack for N frames)
 * If no entity in wind-up, or player is late:
 *   → parry fails, player takes damage if entity is ATTACKING
 */
void    check_parry(t_game *game)
{
    t_entity    *e;

    e = game->entities;
    while (e)
    {
        if (e->state == ATTACK_WINDUP)
        {
            double dx = e->x - game->pos_x;
            double dy = e->y - game->pos_y;
            if (sqrt(dx*dx + dy*dy) < PARRY_RANGE)
            {
                e->state = STUNNED;
                e->stun_frames = STUN_DURATION;
                // play parry success sound / flash
                return ;
            }
        }
        e = e->next;
    }
    // No valid parry target — do nothing (or deal chip damage to player)
}
```

> **Tuning the feel:** The wind-up window should be visible enough for the player to react (at 30fps, 6–10 frames = 0.2–0.3 seconds) but short enough to require attention. Start at 8 frames and adjust based on feel.

---

## Exit Checkpoint

- [ ] Weapon sprite renders at bottom-center of screen
- [ ] Attack animation plays through all states in sequence
- [ ] Melee hits entities within range and attack cone
- [ ] Entity transitions HIT → DEAD correctly with animation
- [ ] Parry during wind-up cancels entity attack and plays stagger animation
- [ ] Parry during active attack phase deals damage to player
- [ ] Both states are visually distinct from player's POV
