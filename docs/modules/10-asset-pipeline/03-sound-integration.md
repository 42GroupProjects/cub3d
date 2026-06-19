# 03 — Exploring Sound Expansion

Cub3D does not explicitly mandate sound, but a game engine without audio feels dead. The easiest way to get audio working for the 42 Evaluation without violating the "No external libraries except MLX" rule is to hijack the operating system's native command-line audio players.

## The `system()` Trick
In C, you can use the `stdlib.h` `system()` function to execute bash commands. 

On macOS, Apple provides an invisible command-line music player called `afplay`. On Linux, there is `aplay` or `paplay`.

```c
void play_sound_mac(char *filepath)
{
    char command[256];
    
    // We add an ampersand '&' so the bash command executes in the background
    // If you forget the '&', your entire game will freeze until the sound finishes playing!
    sprintf(command, "afplay %s &", filepath);
    
    system(command);
}
```

### Implementing it in Combat Lab
```c
if (keycode == MOUSE_LEFT && game->weapon.state == W_IDLE)
{
    play_sound_mac("./sounds/sword_swing.wav");
    game->weapon.state = W_WINDUP;
}
```

## Advanced Option: MiniAudio
If you want precise volume control, distance-based attenuation (sound gets quieter the further away the duck is), or the ability to stop playing sounds midway, the `afplay` trick will not suffice.

You would need to import a Single-Header library like **MiniAudio** (`miniaudio.h`). It is written entirely in standard C. 

However, **check with your 42 campus strictness**. Most campuses do not allow `miniaudio.h` unless specifically given permission for the Bonus phase, as standard rules strictly state "Only the math library and MiniLibX are allowed". Always stick to `afplay` / `system()` if in doubt!
