# Module 12 helper — Valgrind (parser + game exit)

Quick commands so freeze leak checks do not depend on guessing flags.

## Parser only (no window) — already automated

```bash
make test-maps
make valgrind-parser
```

## Game exit paths (manual — after textures work)

```bash
# Build with symbols
make debug

# Run under valgrind, then press ESC (or click window X)
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	./cub3d maps/m_valid/01_minimal_N.cub
```

Expect: no **definitely lost** blocks from map/texture strings after `clean_exit`.

MLX often leaves **still reachable** blocks from X11 — that is normal; focus on
definitely/indirectly lost from your `malloc`s.

## What `clean_exit` does

`clean_exit(cub, code)` (in `src/parser/free_map.c`):

1. `free_cub_struct` — MLX image/window/display + player + ray  
2. `free_config` — texture path strings + map grid  
3. `exit(code)`

ESC / window X already call it via `on_close`. When you add wall `mlx` images,
destroy them inside `free_cub_struct` **before** `mlx_destroy_display`.
