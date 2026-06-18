# 02 — Architecting the Entity State Machine

An entity that just loops the same animation is a prop, not an active enemy. To make gameplay reactive, we need an Artificial Intelligence concept known as a **Finite State Machine (FSM)**.

At its core, an FSM means the Duck can only be in one predefined State at any given moment. Depending on external variables (like distance to the player), it can Transition from one State to another.

## The Duck Logic Flow
Here is the intended Combat Lab FSM for the Duck:

1. **`STATE_IDLE`**: The default state. Plays the `idle` animation loop. 
   - *Condition:* If Euclidean distance to player < Aggro Radius -> Transition to `STATE_CHASE`.
2. **`STATE_CHASE`**: The Walk state. Plays the `walk` animation. Moves its `entity[x/y]` towards the player mathematically every frame.
   - *Condition:* If distance to player < Melee Radius -> Transition to `STATE_ATTACK`.
   - *Condition:* If distance to player > Aggro Radius -> Transition back to `STATE_IDLE`. *(De-aggro)*
3. **`STATE_ATTACK`**: The active aggression state. Plays the `attack` animation one time (no looping).
   - *Condition:* If animation finishes -> Deal damage to player -> Return to `STATE_CHASE`.
   - *Interrupt Condition:* If Player executes a perfect parry -> Nullify damage -> Transition to `STATE_STUNNED`.
4. **`STATE_STUNNED`**: Pauses and shakes for X seconds before reverting to `STATE_CHASE`.
5. **`STATE_DEAD`**: Disables all AI logic. Shrinks or plays a final `die` animation. Raycasting collider becomes transparent.

## The C Implementation Structure

We run a single central `update_entities()` function every frame. It loops through all loaded entities and runs logic based on a `switch/case` pattern (or multiple `if/else`).

```c
typedef enum e_state {
    IDLE = 0,
    CHASE,
    ATTACK,
    STUNNED,
    DEAD
} t_state;

void update_entity_ai(t_game *g, t_entity *e, double dt)
{
    // Skip logic for dead bodies
    if (e->state == DEAD) return;

    // Calculate distance to player globally once per entity
    double dx = g->pos_x - e->x;
    double dy = g->pos_y - e->y;
    double dist = sqrt(dx*dx + dy*dy);

    if (e->state == IDLE)
    {
        if (dist <= 6.0) // Aggro Radius
            e->state = CHASE;
    }
    else if (e->state == CHASE)
    {
        if (dist <= 1.0) // Melee Hit Range
            e->state = ATTACK;
        else if (dist > 7.0) // Lost interest
            e->state = IDLE;
        else
        {
            // Move mathematically closer
            e->x += (dx / dist) * ENEMY_SPEED * dt;
            e->y += (dy / dist) * ENEMY_SPEED * dt;
        }
    }
    else if (e->state == ATTACK)
    {
        // ... play animation, fire event ...
    }
}
```

By ensuring this is run *before* the rendering loop kicks off, the projection mathematics will perfectly map onto the updated XYZ positions, and correctly load the sprite frame associated with `e->state`!
