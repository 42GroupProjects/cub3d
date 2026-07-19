# Textures (module 10 — assets only)

Temporary wall textures for cub3D (MiniLibX `.xpm`).

| File | Role | Look |
|------|------|------|
| `north.xpm` | NO | Red brick + N mark |
| `south.xpm` | SO | Green moss + S mark |
| `west.xpm` | WE | Blue stone + W mark |
| `east.xpm` | EA | Sand / gold + E mark |

All four are 64×64 procedural XPMs (no third-party license). Replace anytime;
keep `.cub` header paths in sync (`./textures/north.xpm`, …).

## For the game partner

You only need to:

1. `mlx_xpm_file_to_image` each path from `c->config->no_tex` / `so` / `we` / `ea`
2. Sample pixels in the wall draw loop
3. `mlx_destroy_image` those four in `free_cub_struct` before `mlx_destroy_display`

Asset conversion / nicer art can stay offline — see `docs/modules/10-asset-pipeline/`.
