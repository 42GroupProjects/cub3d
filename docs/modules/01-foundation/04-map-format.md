# 04 — Map Format: Grid Representation in Memory

## Outcome
You can represent a `.cub` map as a clean 2D array in C, access it correctly, and understand the edge cases that will cause crashes if you skip them.

---

## 1. The Map as a 2D `char` Array

Your internal map representation should be a `char **` (an array of strings):

```c
typedef struct s_map {
    char    **grid;     // arr[y][x]
    int     width;      // maximum row length (important!)
    int     height;     // number of rows
    int     spawn_x;    // integer grid coordinate
    int     spawn_y;    // integer grid coordinate
    char    spawn_dir;  // 'N', 'S', 'E', 'W'
} t_map;
```

Each `grid[y]` is a `char *` pointing to one row. `grid[y][x]` gives the cell character.

---

## 2. Valid Characters

| Char | Meaning |
|------|---------|
| `1` | Wall |
| `0` | Floor (walkable) |
| ` ` | Void (outside the map shape) |
| `N/S/E/W` | Spawn point with facing direction |

The map does **not** need to be rectangular. Different rows can have different lengths. This is intentional and it is where most crashes come from.

---

## 3. The Width Problem

```c
// THIS IS A BUG:
for (int x = 0; x < map->width; x++)
    process(map->grid[y][x]);  // might read beyond end of a short row!
```

You have two correct approaches:

**Option A: Normalize the grid** — pad all short rows with spaces up to `width`:
```c
// After parsing: for every row, ft_realloc to max_width, fill with ' '
```

**Option B: Guard every access with row length**:
```c
// Track row lengths explicitly
if (x < (int)ft_strlen(map->grid[y]))
    c = map->grid[y][x];
else
    c = ' '; // treat short rows as void
```

Option A makes the rest of the code simpler. Choose it.

---

## 4. Coordinate System

Map coordinates are integers `(x, y)` where:
- `x` increases **East** (right)
- `y` increases **South** (down)

Player floats use the same system: `pos_x = 3.5` means the player is halfway through column 3.

```
map->grid[0][0]  = top-left cell
map->grid[0][1]  = one step East
map->grid[1][0]  = one step South
```

Converting between player float position and grid index:
```c
int grid_x = (int)player->pos_x;  // truncate, not round
int grid_y = (int)player->pos_y;
```

> [!IMPORTANT]
> Use `(int)` (truncate toward zero), not `round()`. The cell you occupy is always
> the one your integer coordinate falls in.

---

## 5. Cell Size and Screen Mapping (2D debug view)

When drawing the 2D grid to screen for the debug view (Module 3), the relationship is:

```c
// From so_long's calc_cell_dim_and_offset:
int cell_w = canvas_width  / map_width;
int cell_h = canvas_height / map_height;
int cell_dim = (cell_w < cell_h) ? cell_w : cell_h;  // take smaller to preserve ratio

// Center the grid on canvas with offsets:
int offset_x = (canvas_width  - cell_dim * map_width)  / 2;
int offset_y = (canvas_height - cell_dim * map_height) / 2;

// Screen position of cell (x, y):
int screen_x = offset_x + x * cell_dim;
int screen_y = offset_y + y * cell_dim;
```

This is exactly how `so_long` positioned its cells. Adapt it for Cub3D's debug view.

---

## 6. Sample Maps (add to `tests/valid/`)

### Minimum valid map (`tests/valid/minimal.cub`)
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 50,50,50
C 30,30,200

111111
100001
100N01
100001
111111
```

### Non-rectangular map (`tests/valid/nonrect.cub`)
```
...same header...

1111111111
1000000001
10000N0001
100001
1111111111
```

Row 4 is shorter than the others. Your parser and all accessors must handle this without segfaulting.

---

## 7. Build Exercise

Write a function `print_map(t_map *map)` that:
1. Prints each row using `write(STDOUT_FILENO, ...)`.
2. Marks the spawn position with a `@` symbol.
3. Prints the `width` and `height` values.

Then in `examples/02_parse_header`, call it on 3 maps of different shapes and confirm the output matches the file.

---

## Checkpoint

- [ ] Map is stored as `char **grid` with `height` rows
- [ ] All rows are padded to `width` with spaces
- [ ] Spawn coordinates and direction are extracted from the grid
- [ ] `print_map` shows a correct representation of at least 3 maps
- [ ] A non-rectangular map does not crash `print_map`

## Crisis Card

| Symptom | Fix |
|---------|-----|
| Segfault accessing `grid[y][x]` | Row `y` has fewer than `x+1` characters; you need padding. |
| Spawn position is `(0, 0)` always | The loop that sets spawn isn't finding N/S/E/W in the right pass. |
| Width reports wrong value | Width must be the **maximum** row length, not the first row's length. |

## Review Questions

1. If a row is 5 chars wide and `width` is 10, what character fills positions 5–9?
2. Why do we use truncation (`(int)`) rather than `round()` when converting float position to grid index?
3. `so_long` computed `cell_dim` as the minimum of width-based and height-based cell sizes. Why the minimum and not the maximum?

## Journal Prompt

Write in your journal:
- Draw a 5×5 map on paper. Label every cell as wall, floor, void, or spawn.
- Describe in writing what `grid[3][2]` means (row 3, column 2) and which direction from spawn that lands.
- What would your code do with a map that is 1 cell wide?
