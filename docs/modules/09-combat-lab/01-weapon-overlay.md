# 01 — Weapon GUI Overlay

To create a visceral first-person experience, mounting a weapon sprite directly onto your screen completes the illusion. The weapon is entirely divorced from 3D projected mathematics; it is a fixed, 2D overlay drawn directly onto the image buffer at the end of every frame.

## The Theory
Because the weapon is glued to the camera, it never moves in X/Y 3D space. Instead, it plays different animation frames based on the player's internal state (e.g., Idle, Swinging, Recovering, Parrying).

## The State Engine
Just like the enemies, the Weapon needs a Finite State Machine (FSM).

```c
typedef enum e_weapon_state {
    W_IDLE = 0,
    W_WINDUP,     // Drawing back for a swing
    W_ACTIVE,     // Swing executes (Damage frames)
    W_RECOVERY    // Swing follow-through
} t_wstate;
```

We transition these states using player input.
```c
// Inside your mouse/key hook
if (keycode == MOUSE_LEFT_CLICK && weapon.state == W_IDLE)
{
    weapon.state = W_WINDUP;
    weapon.timer = 0.0;
}
```

## The Rendering Hook
After `draw_walls()` and `draw_sprites()` have filled the image buffer, call `draw_weapon_overlay()` before pushing the image to the window.

```c
void draw_weapon_overlay(t_game *game)
{
    // Define position (e.g., bottom-right corner)
    int align_x = SCREEN_W / 2; // Center horizontally
    int align_y = SCREEN_H - weapon_texture_height; // Align to bottom

    t_image *current_sprite;

    // Pick sprite based on state and timer
    if (game->weapon.state == W_IDLE)
        current_sprite = game->tex_weapon_idle;
    else if (game->weapon.state == W_WINDUP)
        current_sprite = game->tex_weapon_windup;
    // ... etc ...

    // Blit the image using standard put_pixel logic, ignoring transparent pixels!
    blit_texture(game->img, current_sprite, align_x, align_y);
}
```

## Creating "Weapon Sway" (Bobbing)
A static weapon looks rigid. You can make the weapon "sway" mathematically without needing a dozen different animation frames, just by modifying `align_x` and `align_y` based on checking if the player is currently moving.

```c
// Simplistic view bobbing using a sine wave
if (player_is_walking)
{
    game->weapon.bob_timer += delta_time * 10.0;
    align_x += sin(game->weapon.bob_timer) * 15.0; // Sway left/right rapidly
    align_y += cos(game->weapon.bob_timer * 2.0) * 8.0; // Bounce up/down
}
else
{
    // Slowly reset bob_timer back to 0 so it aligns to center
    game->weapon.bob_timer *= 0.9;
}
```
