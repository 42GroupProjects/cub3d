# Best Practices & Common Pitfalls

Building a 3D engine in pure C requires strict architectural discipline. A rendering bug is easy to see and fix; a spaghetti-code memory leak will fail your 42 evaluation instantly.

Below is the definitive guide to Good vs. Bad practices in the Cub3D project, strictly adhering to the 42 Norm and industry stability standards.

---

## 1. Architecture & The Norm

### The "God Loop" Problem
The most common structural mistake students make is dumping all their logic into the single `mlx_loop_hook`. Wait until you try to pass the 25-line-per-function limit with that design.

> **❌ Bad Practice:**
> ```c
> int game_loop(t_game *game)
> {
>     // 50 lines of input handling
>     if (game->keys.w) { ... }
>     
>     // 100 lines of DDA Math
>     int x = 0;
>     while (x < SCREEN_W) { ... }
>     
>     // 30 lines of pixel pushing
>     mlx_put_image_to_window(game->mlx, game->win, ...);
>     return (0);
> }
> ```

> **✅ Good Practice:**
> Create a hierarchical design. The loop should only call high-level managers.
> ```c
> int game_loop(t_game *game)
> {
>     update_player_state(game);    // Handle movement & collisions
>     clear_image_buffer(game);     // Reset offscreen canvas
>     cast_all_rays(game);          // DDA and Wall rendering
>     render_sprites(game);         // Painter's algorithm
>     flush_to_screen(game);        // mlx_put_image_to_window
>     return (0);
> }
> ```

---

## 2. Memory & Error Handling

### The "Rage Quit" Parse Failure
When parsing the `.cub` configuration file, failing gracefully is mandatory. If the map string reads invalid data midway, simply calling `exit(1)` ensures Valgrind will complain about leaked strings and file descriptors.

> **❌ Bad Practice:**
> ```c
> int fd = open(filename, O_RDONLY);
> char *line = get_next_line(fd);
> if (!is_valid_map_char(line[0]))
> {
>     printf("Error\nInvalid map\n");
>     exit(1); // Leaks 'line', leaks 'fd', leaks 't_game'
> }
> ```

> **✅ Good Practice:**
> Use a centralized Garbage Collector / Exit function. Pass `error_codes` and always clean up.
> ```c
> int fd = open(filename, O_RDONLY);
> char *line = get_next_line(fd);
> if (!is_valid_map_char(line[0]))
> {
>     free(line);
>     close(fd);
>     cub3d_exit(game, ERR_INVALID_MAP); // This function frees everything and cleanly exits
> }
> ```

---

## 3. Graphics & Performance

### The "Pixel Put" Trap
The original `mlx_pixel_put()` pushes a single pixel directly completely bypassing the GPU batching process. Drawing an 800x600 screen using this function means issuing 480,000 independent window draw calls *per frame*. Your game will run at 2 FPS.

> **❌ Bad Practice:**
> ```c
> int x = 0;
> while (x < SCREEN_W)
> {
>     int y = drawStart;
>     while (y < drawEnd)
>     {
>         mlx_pixel_put(game->mlx, game->win, x, y, color); // DISASTER!
>         y++;
>     }
>     x++;
> }
> ```

> **✅ Good Practice:**
> Double Buffering! Write to an allocated memory buffer (RAM), then push the entire completed image to the screen exactly once per frame.
> ```c
> void safe_pixel_put(t_img *img, int x, int y, int color)
> {
>     int offset = (y * img->line_len) + (x * (img->bpp / 8));
>     *(unsigned int *)(img->pixels + offset) = color; // Write to RAM instantly
> }
> // ... later in the main loop ...
> mlx_put_image_to_window(game->mlx, game->win, img->img_ptr, 0, 0);
> ```

---

## 4. Modularity & Maintainability

### "Magic Numbers"
When you write arbitrary numbers into equations, returning to fix them two weeks later is a nightmare. What does `1.5` mean? What does `0.05` mean?

> **❌ Bad Practice:**
> ```c
> if (dist < 1.5) { game->state = 3; }
> player_angle += 0.05;
> ```

> **✅ Good Practice:**
> Define everything in a central `includes/macros.h` file.
> ```c
> // macros.h
> #define RANGE_MELEE_HIT  1.5f
> #define SPEED_ROTATION   0.05f
> #define STATE_ATTACKING  3
> 
> // main.c
> if (dist < RANGE_MELEE_HIT) { game->state = STATE_ATTACKING; }
> player_angle += SPEED_ROTATION;
> ```

By adhering to these four principles, your codebase will be immune to 90% of the common pitfalls that cause failure during evaluations.

---

## 5. Entity Management

### Dynamic `malloc` in the Game Loop
A classic junior mistake is allocating enemy and entity objects inside the game loop when they "spawn."

> **❌ Bad Practice:**
> ```c
> int game_loop(t_game *game)
> {
>     if (should_spawn_duck)
>     {
>         t_duck *d = malloc(sizeof(t_duck)); // malloc INSIDE the game loop!
>         d->pos_x = 4.5;
>         // ... append to linked list ...
>     }
>     return (0);
> }
> ```

> **✅ Good Practice:**
> Pre-allocate a fixed-size static array at startup. Activate slots by flipping a boolean flag — zero `malloc` during active play.
> ```c
> // At init time: scan the map for 'D' (duck) characters
> void init_entities(t_game *game)
> {
>     int i = 0;
>     // ... scan map...
>     game->entities[i].active = 1;
>     game->entities[i].pos_x = x + 0.5;
>     game->entities[i].pos_y = y + 0.5;
>     // Never malloc again during gameplay.
> }
> ```

---

## 6. Memory Teardown & Valgrind Zero Leaks

### The "Partial Clean" Pattern
Many students write a `clean_exit` that frees game structs but forgets to destroy MLX images or close file descriptors.

> **❌ Bad Practice:**
> ```c
> void clean_exit(t_game *game)
> {
>     free(game->map.grid); // Forgets MLX images, window, display
>     exit(0);
> }
> ```

> **✅ Good Practice:**
> A strict layered teardown — Graphics, System, Memory — in exactly that order.
> ```c
> void clean_exit(t_game *game)
> {
>     // 1. Destroy all MLX images
>     if (game->img.img_ptr)
>         mlx_destroy_image(game->mlx, game->img.img_ptr);
>
>     // 2. Destroy window and display connection
>     if (game->win)
>         mlx_destroy_window(game->mlx, game->win);
>     #ifdef __linux__
>     if (game->mlx)
>     {
>         mlx_destroy_display(game->mlx);
>         free(game->mlx);
>     }
>     #endif
>
>     // 3. Free map memory last (after MLX is done)
>     free_map(game->map.grid);
>     exit(0);
> }
> ```

---

## 7. Norminette-Specific Reminders

| Rule | Bad | Good |
|------|-----|------|
| Max function length | Functions over 25 lines | Extract helpers: `init_sidedist()`, `do_dda_step()` |
| No `for` loops | `for (int i = 0; i < N; i++)` | `i = 0; while (i < N) { ... i++; }` |
| Variable declarations | Declare inside an `if` block | Declare ALL variables at the top of the function |
| No inline comments | `color = color / 2; // darken` | Block comment above the function explaining the formula |
| Max line length | Lines over 80 characters | Split into multi-line with alignment padding |
| Global variables | `int g_count = 0;` in `.c` | Only `const` or `static` globals allowed; prefix with `g_` |
