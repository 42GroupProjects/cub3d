# 02 — Melee Hit Detection

Unlike a projectile weapon that requires raycasting (a bullet ray), melee combat relies on calculating volumetric distances and viewing angles. You only hit enemies if they are:
1. Close enough to you (Radius Check).
2. Generally in front of you (Angle Check).

## 1. The Distance Check (Euclidean)
When the active attack frame hits, iterate through your active dynamic entities array.

```c
#define MELEE_RANGE 1.5 // Distance in grid cells

double dx = entity->x - game->pos_x;
double dy = entity->y - game->pos_y;

// Standard Euclidean Distance
double dist = sqrt(dx*dx + dy*dy);

if (dist > MELEE_RANGE)
    return; // Missed: Enemy too far away.
```

## 2. The Hit Cone Check (Atan2)
If the enemy is within 1.5 units, we must verify the player is actually looking at them. We calculate the angle between the player's viewing angle vector and the vector pointing directly to the entity.

```c
#define MELEE_CONE_RAD (M_PI / 4) // 45 degree hit cone (22.5 left, 22.5 right)

// Calculate the absolute world angle to the enemy
double angle_to_enemy = atan2(dy, dx);

// Calculate the absolute world angle of the player's viewpoint
double player_angle = atan2(game->dir_y, game->dir_x);

// Calculate the delta (relative angle)
double angle_diff = angle_to_enemy - player_angle;

// Normalize the angle to be strictly between -PI and +PI
while (angle_diff > M_PI)  angle_diff -= 2 * M_PI;
while (angle_diff < -M_PI) angle_diff += 2 * M_PI;

// Check if they fall within the cone
if (fabs(angle_diff) <= MELEE_CONE_RAD)
{
    // HIT CONFIRMED! 
    deal_damage_to_entity(entity);
    spawn_blood_particle(entity);
}
```

## 3. Wall Occlusion Check (Rays!)
If there is a Duck behind a solid wall but only 1 unit away from you, and you swing your sword, the code above will register a hit through the wall.

To prevent this, you can fire a very short DDA ray explicitly toward the enemy's coordinates. If that single ray hits a `'1'` wall *before* its internal ray distance reaches the enemy distance, you cancel the hit.

## ⚠️ Common Melee Logic Mistakes

**Mistake 1: Forgetting angle normalization — the 360° wrap bug (❌)**

```c
/* WRONG — angle_diff can be outside [-PI, +PI] */
double angle_diff = angle_to_enemy - player_angle;

// If player faces 170° and enemy is at -170°, the raw diff is 340°.
// fabs(340°) > MELEE_CONE_RAD, so the hit is REJECTED.
// But visually, the enemy is directly in front of you!
if (fabs(angle_diff) <= MELEE_CONE_RAD)
    deal_damage_to_entity(entity);
```

The angle difference can fall outside `[-π, +π]` when the player is near the 180°/−180° boundary (looking almost directly West). `340°` and `-20°` describe the exact same direction, but without normalization, `fabs(340° * PI/180)` is ~5.9 radians — far outside any melee cone.

**Fix:** Normalize the angle after subtraction:
```c
double angle_diff = angle_to_enemy - player_angle;

// Wrap to [-PI, +PI]
while (angle_diff >  M_PI) angle_diff -= 2 * M_PI;
while (angle_diff < -M_PI) angle_diff += 2 * M_PI;

if (fabs(angle_diff) <= MELEE_CONE_RAD)
    deal_damage_to_entity(entity);
```

---

**Mistake 2: Hitting enemies through walls (❌)**

```c
/* WRONG — only distance and angle checked, no wall occlusion */
if (dist <= MELEE_RANGE && fabs(angle_diff) <= MELEE_CONE_RAD)
    deal_damage_to_entity(entity); // Hits the duck through the wall!
```

If a duck is 1.2 units away but separated from the player by a solid wall, both the distance check (< 1.5) and the angle check pass. The player can "swing through the wall" and kill the duck invisibly.

**Fix:** After passing distance + angle checks, cast a short DDA ray toward the enemy. Cancel the hit if the ray hits a wall before reaching the enemy's distance:
```c
double wall_dist = cast_ray_toward(game, dx / dist, dy / dist);
if (wall_dist < dist)
    return ; // Wall is between player and enemy — blocked!
deal_damage_to_entity(entity);
```

---

**Mistake 3: Using `sqrt()` every frame for all entities (❌)**

```c
/* SLOW — calling sqrt for 50 enemies every frame, even when not attacking */
for (int i = 0; i < game->entity_count; i++)
{
    double dx = entities[i].x - game->pos_x;
    double dy = entities[i].y - game->pos_y;
    double dist = sqrt(dx*dx + dy*dy); // 50 sqrt() calls per frame!
    if (dist < AGGRO_RANGE)
        set_aggro(&entities[i]);
}
```

`sqrt()` is expensive. At 60 FPS with 50 enemies, that is 3,000 `sqrt()` calls per second just for aggro range checks.

**Fix:** Compare squared distances. `sqrt(a) < sqrt(b)` is always equivalent to `a < b`:
```c
double dist_sq = dx*dx + dy*dy;
// AGGRO_RANGE_SQ = AGGRO_RANGE * AGGRO_RANGE (pre-calculated once)
if (dist_sq < AGGRO_RANGE_SQ)
    set_aggro(&entities[i]);
// Only call sqrt() when you actually need the exact distance value (e.g. for damage scaling)
```
