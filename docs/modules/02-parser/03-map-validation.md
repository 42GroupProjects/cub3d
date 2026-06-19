# 02-03 — Map Validation: Wall Enclosure

The map must be fully enclosed by walls. Any floor cell (`0`) or spawn cell that could "escape" to the map edge is a fatal error.

---

```mermaid
flowchart TD
    Start[Start Map Validation] --> CheckChars{Contains only<br/>'0','1',' ','N','S','E','W'?}
    CheckChars -- No --> ErrorChar[Error: Invalid Character] --> Exit[exit(1)]
    
    CheckChars -- Yes --> CheckSpawn{Exactly ONE<br/>Spawn Character (\'N\', \'S\', \'E\', \'W\')?}
    CheckSpawn -- No --> ErrorSpawn[Error: Zero or Multiple Spawns] --> Exit
    
    CheckSpawn -- Yes --> ExtractPlayer[Store Player X, Y, and Direction]
    
    ExtractPlayer --> StartFloodFill[Execute Flood Fill]
    StartFloodFill --> EvalFlood{Did Flood Fill leak<br/>out of bounds?}
    
    EvalFlood -- Yes --> ErrorLeak[Error: Map not enclosed by walls] --> Exit
    EvalFlood -- No --> Success[Map is Valid!\nProceed to MLX Init]
```

## The Problem Visual


```
111111
100001
100001
100N00   ← '0' on the right edge — this leaks!
111111
```

The rightmost `0` on row 3 is adjacent to nothing (beyond it is the end of the string, treated as void). This map must be **rejected**.

---

## The mathias-mrsn Approach: Border Cross-Check

mathias-mrsn's `check_map.c` uses a clever technique: it creates a padded copy of the map where every space and border cell becomes `'x'` (a sentinel), then checks that no `'x'` cell is adjacent to a non-wall, non-x cell.

```c
/* mathias-mrsn/srcs/parsing/check_map.c — border sentinel approach */

/*
 * __space_to_cross__: converts each row to a bordered version.
 * Spaces become 'x' (void sentinel).
 * NSEW spawn chars become '0' (floor, to simplify neighbor checks).
 * The function wraps each row with 'x' on both ends.
 */
char    *__space_to_cross__(char *str, size_t len)
{
    char      *new_str;
    size_t    idx;
    size_t    str_idx;

    new_str = __malloc(sizeof(char) * len + 3, RAYCASTING_STACK);
    str_idx = 0;
    idx = 0;
    new_str[idx++] = 'x';          // left border sentinel
    while (str[str_idx])
    {
        if (str[str_idx] == ' ')
            new_str[idx] = 'x';    // space → sentinel
        else if (__is_charset(str[str_idx], "NSEW"))
            new_str[idx] = '0';    // spawn → floor (for checking purposes)
        else
            new_str[idx] = str[str_idx];
        str_idx++;
        idx++;
    }
    while (idx < len + 2)
        new_str[idx++] = 'x';      // right padding → sentinel
    new_str[idx] = 0;
    return (new_str);
}

/*
 * check_cross: for every 'x' cell, check all 4 neighbors.
 * If any neighbor is not '1' and not 'x', there is a leak → error.
 */
void    check_cross(t_cub *s, char **strs,
    const size_t _height, const size_t _width)
{
    size_t    i;
    size_t    j;

    i = 0;
    while (i < _height)
    {
        j = -1;
        while (++j < _width)
        {
            if (strs[i][j] == 'x')
            {
                if (j < _width - 1 > 0 && strs[i][j + 1] != '1'
                        && strs[i][j + 1] != 'x')
                    __leak_on_map_error__(s, i - 1, j + 1);
                if (i > 0 && strs[i - 1][j] != '1' && strs[i - 1][j] != 'x')
                    __leak_on_map_error__(s, i - 2, j - 1);
                if (i < _height - 1 && strs[i + 1][j] != '1'
                        && strs[i + 1][j] != 'x')
                    __leak_on_map_error__(s, i, j - 1);
                if (j > 0 && strs[i][j - 1] != '1' && strs[i][j - 1] != 'x')
                    __leak_on_map_error__(s, i - 1, j - 2);
            }
        }
        i++;
    }
}

void    check_map_walls(t_cub *s)
{
    char           **strs;
    const size_t    _size     = __strslen(s->parser->map);
    const size_t    _size_max = __strs_maxlen__(s->parser->map);
    size_t          idx;

    strs = __malloc(sizeof(char *) * (_size + 3), RAYCASTING_STACK);
    idx = 0;
    strs[idx++] = __full_cross_line__(_size_max);  // top border row of 'x'
    while (idx < _size + 1)
    {
        strs[idx] = __space_to_cross__(s->parser->map[idx - 1], _size_max);
        idx++;
    }
    strs[idx] = __full_cross_line__(_size_max);    // bottom border row of 'x'
    check_cross(s, strs, __strslen(strs), __strlen(strs[0]));
}
```

> **How to read this:** After `check_map_walls` runs, the entire map has a border of `'x'` around it. Every `'x'` cell represents void (edge or space). The `check_cross` function then verifies: every `'x'` cell's 4 neighbors must be either `'1'` (wall) or `'x'` (more void). If any neighbor is `'0'` or a spawn character, there is a floor-to-void leak — error.

---

## Alternative: Simple Neighbor Check

A simpler implementation without the sentinel layer: for every `'0'` and spawn cell, check all 4 neighbors. If any neighbor is out of bounds or is `' '`, reject the map.

```c
/* Simple 4-neighbor wall check — implement this first */
int    check_cell_surrounded(char **grid, int y, int x, int height, int width)
{
    if (y <= 0 || y >= height - 1 || x <= 0 || x >= width - 1)
        return (0);  // floor cell on border → open
    if (grid[y - 1][x] == ' ' || grid[y + 1][x] == ' ' ||
        grid[y][x - 1] == ' ' || grid[y][x + 1] == ' ')
        return (0);  // neighbor is void → open
    return (1);
}

int    validate_map_closure(t_map *map)
{
    int    y;
    int    x;

    y = 0;
    while (y < map->height)
    {
        x = 0;
        while (x < map->width)
        {
            if (map->grid[y][x] == '0' || ft_strchr("NSEW", map->grid[y][x]))
            {
                if (!check_cell_surrounded(map->grid, y, x,
                        map->height, map->width))
                {
                    ft_putendl_fd("Error\nMap is not fully enclosed!", 2);
                    return (0);
                }
            }
            x++;
        }
        y++;
    }
    return (1);
}
```

> **Use this simpler version first.** It is easier to debug. It passes all standard test cases. Add the sentinel approach only if edge cases require it.
