*This project has been created as part of the 42 curriculum by thanh-ng, lwittwer.*

# cub3D

## Description

cub3D is a Wolfenstein 3D-inspired raycaster. It parses a `.cub` scene file
(textures, floor/ceiling colors, and a closed map) and renders a first-person
view of a maze with MiniLibX. Wall faces use distinct North/South/East/West
textures; movement uses WASD and looking uses the arrow keys.

## Instructions

### Build

```bash
make
```

Requires MiniLibX (`libmlx`), X11, and the vendored `libft`.

```bash
make clean    # remove object files
make fclean   # remove objects + binary
make re       # rebuild from scratch
make bonus    # same as all (bonus features built into the binary)
```

### Run

```bash
./cub3D maps/m_valid/01_minimal_N.cub
```

Exactly one argument: path to a `.cub` map. On invalid input the program prints
`Error` followed by a message on stderr and exits non-zero.

### Tests

```bash
make test-maps          # valid/invalid map suites (parse-only binary)
make valgrind-parser    # same under valgrind
make valgrind-mlx       # MLX init + clean exit under valgrind
```

### Controls

| Key | Action |
|-----|--------|
| `W` / `S` | Move forward / backward |
| `A` / `D` | Strafe left / right |
| `←` / `→` | Look left / right |
| `M` | Toggle full-map overlay (bonus) |
| `ESC` | Quit cleanly |
| Window close (X) | Quit cleanly |

A small radar minimap sits in the top-left (bonus). Press `M` for a
semi-transparent full 2D map overlay.

### Map format (`.cub`)

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
one spawn among `N` `S` `E` `W`. Example maps live under `maps/m_valid/` and
`maps/m_invalid/`.

## Resources

- [Lode's Computer Graphics Tutorial — Raycasting](https://lodev.org/cgtutor/raycasting.html)
- [F. Permadi — Ray-Casting Tutorial](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)
- [MiniLibX (Linux)](https://github.com/42Paris/minilibx-linux)

### AI usage

AI tools (e.g. Cursor) helped draft and maintain this README, the learning
material under `docs/`, and study/reference sheets. Project source code is
written and reviewed by us; AI-assisted text is checked against the
implementation and the subject. We can explain every submitted part without
depending on AI at evaluation time.
