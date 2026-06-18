# 02 — The Zero Leaks Policy

Your evaluation requires you to run your project using `valgrind` (Linux) or `leaks` (macOS). "Definitely lost" memory will result in a 0.

## Complete Teardown Function
You need a central exit point. If the 3D window is open and the user presses ESC, or hits the X button, or a `malloc` fails during parsing, you funnel the exit through exactly one function.

```c
void clean_exit(t_game *game, int exit_code)
{
    if (game->map)
        free_string_array(game->map);
        
    if (game->tex_north.img_ptr)
        mlx_destroy_image(game->mlx, game->tex_north.img_ptr);
    if (game->tex_south.img_ptr)
        mlx_destroy_image(game->mlx, game->tex_south.img_ptr);
    // ... east, west ...

    if (game->img.img_ptr)
        mlx_destroy_image(game->mlx, game->img.img_ptr);
        
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
        
    #ifdef __linux__
    if (game->mlx)
        mlx_destroy_display(game->mlx); // Required on Linux
    #endif

    exit(exit_code);
}
```

## How to test explicitly

On macOS:
```bash
./cub3d maps/valid.cub &
# Get the process ID (PID)
PID=$!
# Run leaks
leaks -q $PID
```
Or utilize the classic trick by adding this at the top of your `main`:
`system("leaks cub3D");` (Make sure to remove this before submitting to the norm!).

## Common Sneaky Leaks
1. **Unclosed File Descriptors**: If you throw an error while checking `.cub`, make sure you `close(fd)`!
2. **GNL Static Overread**: If you break out of reading the file early and GNL still has a static buffer, it will leak. Read it until `EOF` or free the buffer.
3. **Double Freeing MLX**: `mlx_destroy_window` sometimes destroys internal buffers. Do not manually `free()` mlx internals. Just use the destruction functions.
