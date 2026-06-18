# 02-02 — Parser State Machine

Parse the `.cub` file in two sequential phases. Never try to parse the header and the map simultaneously.

## ⚠️ Common Parser Logic Mistakes

These are the most frequent bugs students produce during the parsing phase:

**Mistake 1: Parsing header and map simultaneously (❌)**

```c
/* WRONG — trying to detect both header lines and map lines in one pass */
while ((line = get_next_line(fd)) != NULL)
{
    if (line[0] == 'N' || line[0] == 'S')
        parse_header(line); // what if "N" is a spawn direction in the map?
    else if (line[0] == '1' || line[0] == '0')
        parse_map_row(line);
    free(line);
}
```

The character `'N'` is valid in *both* the header (`NO ./path.xpm`) AND the map grid (North-facing spawn). This logic incorrectly routes map spawn characters into the texture path parser.

**Fix:** Use a strict two-phase approach. Parse headers until you encounter the first map character (`'0'`, `'1'`, or `' '`), then switch modes:
```c
while ((line = get_next_line(fd)) != NULL)
{
    if (!game->map_started && is_header_line(line))
        parse_header(game, line);
    else
    {
        game->map_started = 1;
        append_map_row(game, line); // store verbatim — no interpretation
    }
    free(line);
}
```

---

**Mistake 2: Stripping spaces from map rows (❌)**

```c
/* WRONG — trimming trailing whitespace from map lines */
while (map_row[len - 1] == ' ')
{
    map_row[len - 1] = '\0';
    len--;
}
```

The subject explicitly allows interior void spaces (` `) inside the map shape. Trimming them causes shorter rows, making the 2D grid jagged and breaking the flood fill validation (out-of-bounds reads on shorter rows).

**Fix:** Store every row verbatim. Pad all rows to `max_width` with spaces *after* all rows have been collected, not during per-line parsing.

---

**Mistake 3: Accepting duplicate identifier keys (❌)**

```c
/* WRONG — overwrites the first NO value with any subsequent NO line */
if (ft_strncmp(line, "NO ", 3) == 0)
    game->tex_path_north = ft_strdup(line + 3);
```

A malformed `.cub` file with `NO` listed twice silently overwrites the first value and leaks the first allocated string. Valgrind will report a "definitely lost" block.

**Fix:** Check for duplicates before storing:
```c
if (ft_strncmp(line, "NO ", 3) == 0)
{
    if (game->tex_path_north != NULL) // already set!
        return (parse_error(game, "Duplicate NO identifier"));
    game->tex_path_north = ft_strdup(line + 3);
}
```

---



## Phase 1: Header Parsing

Read lines one at a time. For each line, try to match one of these identifiers:

| Identifier | Expected value |
|------------|---------------|
| `NO` | Path to north texture `.xpm` |
| `SO` | Path to south texture `.xpm` |
| `WE` | Path to west texture `.xpm` |
| `EA` | Path to east texture `.xpm` |
| `F` | Floor color as `R,G,B` |
| `C` | Ceiling color as `R,G,B` |

**Rules:**
- Each identifier can appear **at most once**. Track with a `has_X` flag.
- Blank lines between headers are allowed — skip them.
- A line that starts with a map character (`0`, `1`, ` `, `N`, `S`, `E`, `W`) signals the end of the header phase.

**Reference: derfleck `checker_1.c` argument + file validation:**

```c
/* derfleck/src/checker_1.c — argument and file validation */

int    check_file(char *file)
{
    int    fd;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (0);
    if (close(fd) < 0)
        return (0);
    return (1);
}

int    ends_with(char *str, char *key)
{
    int    start;
    int    i;

    start = (int)(ft_strlen(str) - ft_strlen(key));
    if (start <= 0)
        return (0);
    i = 0;
    while (start && str[start] && key[i] && (str[start] == key[i]))
    {
        start++;
        i++;
    }
    if (i == (int)ft_strlen(key))
        return (1);
    return (0);
}

t_map    *check_input(int argc, char **argv, t_map *map)
{
    if (argc <= 1 || argc > 2 || (argc == 2 && !ends_with(argv[1], ".cub")))
        err_before_mall("Only .cub file required as parameter");
    if (argc == 2 && !check_file(argv[1]))
        err_before_mall("Map file cannot be accessed");
    get_lines(map, argv[1]);
    return (map);
}
```

> **Study:** `ends_with` confirms the `.cub` extension. `check_file` confirms the file is readable. Both run *before* any allocation. Notice how early errors print and exit with no malloc to free.

---

## Phase 2: RGB Color Validation

**Reference: mathias-mrsn `check_specs.c` RGB + duplicate detection:**

```c
/* mathias-mrsn/srcs/parsing/check_specs.c — RGB parser and duplicate guard */

static void    __check_colors__(t_cub *s, char *str, ssize_t idx, uint8_t type_nbr)
{
    char    **strs;

    if (11 < __strlen(str))         // "255,255,255" is 11 chars — reject anything longer
        __color_error__(s, idx);
    strs = __msplit(str, ',', TRASH_STACK);
    if (__strslen(strs) != 3)       // must be exactly 3 components
        __color_error__(s, idx);
    if (3 < __strlen(strs[0]) || 3 < __strlen(strs[1]) || 3 < __strlen(strs[2])
        || !__str_is(strs[0], __IS_DIGIT)   // each component all-digit
        || !__str_is(strs[1], __IS_DIGIT)
        || !__str_is(strs[2], __IS_DIGIT)
        || 255 < __atoi(strs[0])    // each component 0-255
        || 255 < __atoi(strs[1])
        || 255 < __atoi(strs[2]))
        __color_error__(s, idx);
    // store into floor or ceil based on type_nbr
    if (type_nbr != 1)
        s->parser->floor = __create_color(0,
            (uint8_t)__atoi(strs[0]),
            (uint8_t)__atoi(strs[1]),
            (uint8_t)__atoi(strs[2]));
    else
        s->parser->ceil = __create_color(0,
            (uint8_t)__atoi(strs[0]),
            (uint8_t)__atoi(strs[1]),
            (uint8_t)__atoi(strs[2]));
}

void    args_data_is_good(t_cub *s, char *str, uint8_t type_nbr, ssize_t idx)
{
    if (s->parser->check[type_nbr] == __TRUE)  // already seen this identifier
        __duplicate_data_error__(s, idx);
    else
        s->parser->check[type_nbr] = __TRUE;   // mark as seen
    if (type_nbr >= 0 && type_nbr < 2)
        __check_colors__(s, str, idx, type_nbr);
    else
        __check_file__(s, str, idx, type_nbr);  // validate texture path
}
```

> **Key insight:** `check[type_nbr]` is the duplicate-detection mechanism. Before processing any identifier, check if the flag is already set. This is simpler than trying to detect duplicates after the fact.

---

## Phase 3: Map Extraction and Grid Storage

Starting from the first map line:
1. Store each line verbatim (do not strip spaces) into `char **grid`
2. After EOF: `map.height = row count`
3. `map.width = max row length across all rows`
4. Pad every row to `map.width` with spaces: `row[len..width] = ' '`

**Why preserve spaces?** The subject says spaces inside the map shape are valid void cells. They must be treated as `' '` (void), not as errors, and not stripped.

---

## Phase 4: Spawn Detection

Scan the padded grid for exactly one of `N`, `S`, `E`, `W`. Store `spawn_x`, `spawn_y`, and `spawn_dir`.

**Reference: derfleck `checker_2.c` — character validation + spawn count:**

```c
/* derfleck/src/checker_2.c — map character validation + spawn count */

static int    check_valid_chars(char **map)
{
    int    x;
    int    y;

    y = -1;
    while (map[++y])
    {
        x = -1;
        while (map[y][++x])
        {
            if (map[y][x] != '0' && map[y][x] != '1' && map[y][x] != 'W' &&
            map[y][x] != 'S' && map[y][x] != 'E' && map[y][x] != 'N' &&
            map[y][x] != ' ')
                return (0);   // any other char is invalid
        }
    }
    return (1);
}

static int    count_stuff(t_map *map, char c)
{
    unsigned int    i;
    unsigned int    j;
    unsigned int    count;

    count = 0;
    i = -1;
    j = -1;
    while (map->cmap[++i])
    {
        while (map->cmap[i][++j])
        {
            if (map->cmap[i][j] == c)
            {
                map->dir = c;    // also records the spawn direction
                count++;
            }
        }
        j = -1;
    }
    return (count);
}

int    check_map_validity(t_map *map)
{
    int    player_count;

    player_count = 0;
    if (!check_valid_chars(map->cmap))
    {
        ft_putendl_fd("Error\nInvalid character in map!", 2);
        return (0);
    }
    player_count += count_stuff(map, 'S');
    player_count += count_stuff(map, 'W');
    player_count += count_stuff(map, 'E');
    player_count += count_stuff(map, 'N');
    if (player_count != 1)
    {
        ft_putendl_fd("Error\nPlayer count is invalid!", 2);
        return (0);
    }
    return (1);
}
```

> **Notice:** `player_count` adds up all four directions. The check `!= 1` catches both 0 spawns and 2+ spawns. Clean and exhaustive.
