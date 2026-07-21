# Module 12 helper — Valgrind (parser + game exit)

Quick commands so freeze leak checks do not depend on guessing flags.

## 1. Parser only (no window) — automated

```bash
make test-maps
make valgrind-parser
```

Covers every `maps/m_invalid/*.cub` (must fail with `Error`) and
`maps/m_valid/*.cub` under `CUB3D_PARSE_ONLY=1`.

**Status (2026-07-21):** `make valgrind-parser` — all passed.

## 2. MLX init + `clean_exit` (no interactive ESC) — automated

Opens MiniLibX, then tears down via `clean_exit` without entering `mlx_loop`:

```bash
make valgrind-mlx
# equivalent:
# CUB3D_QUIT_AFTER_INIT=1 valgrind --leak-check=full --show-leak-kinds=all \
#   --error-exitcode=42 --track-origins=yes \
#   ./cub3d maps/m_valid/01_minimal_N.cub
```

Headless: the Makefile wraps with `xvfb-run -a` when available.

## 3. Full play session (manual — after textures)

```bash
make debug   # or: make CFLAGS+='-g'

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	./cub3d maps/m_valid/01_minimal_N.cub
# then press ESC (or click window X)
```

Expect: no **definitely lost** / **indirectly lost** from your map strings,
texture path strings, player/ray, or (later) wall texture images.

MLX / X11 often leave **still reachable** blocks — that is normal; ignore those
unless campus says otherwise.

## What `clean_exit` does

`clean_exit(cub, code)` (in `src/parser/free_map.c`):

1. `free_cub_struct` — MLX image/window/display + player + ray  
2. `free_config` — texture path strings + map grid  
3. `exit(code)`

ESC / window X call it via `on_close`.  
`CUB3D_QUIT_AFTER_INIT=1` calls it right after `init_game` (for CI/valgrind).

When you add wall `mlx` images, destroy them inside `free_cub_struct`
**before** `mlx_destroy_display`.
