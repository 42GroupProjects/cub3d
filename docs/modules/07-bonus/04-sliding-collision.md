# 04 — Sliding Wall Collision

To make movement feel professional, hitting a wall at a diagonal angle shouldn't completely lock the player in place. They should slide along the axis that isn't colliding.

## The Theory
We separate the player's intended movement chunk into two discrete steps: evaluating the X movement, and then evaluating the Y movement independently.

If evaluating X results in hitting a wall, we cancel the X movement but keep the Y movement. This naturally results in the player "sliding" along the Y face of the wall.

## The C Implementation

```c
// Define a tiny buffer zone so the player center doesn't clip into the wall mathematically
#define COLLISION_BUFFER 0.1

void move_player_sliding(t_game *game, double move_step_x, double move_step_y)
{
    double new_x = game->pos_x + move_step_x;
    double new_y = game->pos_y + move_step_y;

    // 1. Check X-Axis independently
    // Calculate the map tile the player *would* be in on the X axis, plus our buffer radius
    int map_new_x = (int)(new_x + ((move_step_x > 0) ? COLLISION_BUFFER : -COLLISION_BUFFER));
    int map_current_y = (int)game->pos_y;

    if (game->map[map_current_y][map_new_x] == '0' || is_open_door(game, map_new_x, map_current_y))
    {
        // Safe to move on X axis
        game->pos_x = new_x;
    }

    // 2. Check Y-Axis independently (using the potentially updated pos_x)
    int map_current_x = (int)game->pos_x;
    int map_new_y = (int)(new_y + ((move_step_y > 0) ? COLLISION_BUFFER : -COLLISION_BUFFER));

    if (game->map[map_new_y][map_current_x] == '0' || is_open_door(game, map_current_x, map_new_y))
    {
        // Safe to move on Y axis
        game->pos_y = new_y;
    }
}
```

## How It Works
If the player holds `W` and is walking North-East into a solid East wall:
1. `move_step_x` (East) evaluates. Adding `COLLISION_BUFFER` pushes it over a tile boundary into a `'1'`. The X collision fails. `pos_x` doesn't change.
2. `move_step_y` (North) evaluates. The space directly North of the current `pos_x` is a `'0'`. The Y collision passes. `pos_y` changes.
3. **Result**: The player successfully moved North while holding North-East against an East wall. Sliding achieved!
