# cub3D

*My first RayCaster with miniLibX*

A 42 school project inspired by Wolfenstein 3D: parse a `.cub` scene file and
render a first-person maze view with MiniLibX (raycasting).

## Build

```bash
make
```

Requires MiniLibX (`libmlx`), X11, and `libft` (vendored in this repo).

```bash
make clean    # remove object files
make fclean   # remove objects + binary
make re       # rebuild from scratch
```

## Usage

```bash
./cub3d maps/m_valid/01_minimal_N.cub
```

Exactly one argument: path to a `.cub` map. On invalid input the program prints
`Error` followed by a message on stderr and exits non-zero.

Parser-only smoke test (no window):

```bash
CUB3D_PARSE_ONLY=1 ./cub3d maps/m_valid/01_minimal_N.cub
make test-maps
make valgrind-parser
```

## Controls

| Key | Action |
|-----|--------|
| `W` / `S` | Move forward / backward |
| `A` / `D` | Strafe left / right |
| `←` / `→` | Rotate view (look left / right) |
| `ESC` | Quit |
| Window close (X) | Quit |

## Map format (`.cub`)

Header (any order, before the map):

```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 220,100,0
C 225,30,0
```

Then a closed map grid using `0` (floor), `1` (wall), spaces (void), and exactly
one spawn among `N` `S` `E` `W`.

Example maps live under `maps/m_valid/` and `maps/m_invalid/`.

## Textures

Placeholder wall textures (distinct N/S/E/W brick patterns) are in
`textures/`:

- `textures/north.xpm`
- `textures/south.xpm`
- `textures/west.xpm`
- `textures/east.xpm`

These are simple procedural XPMs for development. You may replace them with any
valid `.xpm` files and point the `.cub` header paths at them.

## Authors

- thanh-ng — parsing / validation
- lwittwer — rendering / input / game loop

## Resources

- [Lode's Computer Graphics Tutorial — Raycasting](https://lodev.org/cgtutor/raycasting.html)
- [F. Permadi — Ray-Casting Tutorial](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)
- [MiniLibX (Linux)](https://github.com/42Paris/minilibx-linux)
