# 06 — Parser Edge Cases & Trap Inputs

The parser is the most rigorous part of your evaluation. Your program must reject **every** invalid input cleanly — printing `Error\n` followed by a descriptive message to `stderr`, then exiting without leaking memory.

The 42 evaluator specifically looks for parser robustness. They will not test just valid maps — they will throw garbage at your binary.

---

## The Complete Trap Catalog

### Category 1: File System Traps

These happen before even opening the file.

**Trap 1.1: Wrong or missing extension**
```bash
./cub3d map          # No extension at all
./cub3d map.txt      # Wrong extension
./cub3d map.cubb     # Extra character
./cub3d              # No argument
./cub3d a b c        # Too many arguments
```

❌ **Bad check:**
```c
if (argc != 2)
    return (error("wrong args"));
// Misses: ./cub3d map.txt still passes argc check
```

✅ **Correct check:**
```c
int check_extension(char *path)
{
    int len;

    len = ft_strlen(path);
    if (len < 5)
        return (0);
    return (ft_strncmp(path + len - 4, ".cub", 4) == 0);
}
```

---

**Trap 1.2: Path is a directory**
```bash
./cub3d /etc          # open() succeeds but read returns EISDIR on Linux
./cub3d ./maps/       # Trailing slash
```

On **macOS**, `open("/etc", O_RDONLY)` returns a valid `fd`. Reading from it returns `-1` with `errno = EISDIR`, causing GNL to silently return `NULL` on the first call — your parser sees an empty file and likely segfaults or accepts it.

On **Ubuntu**, `open("/etc", O_RDONLY)` also succeeds but `read()` fails similarly.

❌ **Bad check:**
```c
int fd = open(path, O_RDONLY);
if (fd < 0)
    return (error("cannot open"));
// Misses: directory fd is valid, read will silently fail
```

✅ **Correct check using `stat`:**
```c
#include <sys/stat.h>

int is_regular_file(char *path)
{
    struct stat st;

    if (stat(path, &st) < 0)
        return (0);
    return (S_ISREG(st.st_mode)); // Returns 0 for directories
}
```

---

**Trap 1.3: File has no read permission**
```bash
chmod 000 map.cub
./cub3d map.cub     # open() returns fd = -1, errno = EACCES
```

Your error message must distinguish "does not exist" from "no permission":
```c
int fd = open(path, O_RDONLY);
if (fd < 0)
{
    if (errno == EACCES)
        ft_putendl_fd("Error\nPermission denied", 2);
    else
        ft_putendl_fd("Error\nFile not found", 2);
    return (0);
}
```

---

**Trap 1.4: Empty file**
```bash
touch empty.cub && ./cub3d empty.cub
```

GNL returns `NULL` immediately. Your parser loop never executes. The `has_all_headers` check at the end catches it:
```c
// After the parse loop:
if (!has_north || !has_south || !has_east || !has_west || !has_floor || !has_ceil)
{
    ft_putendl_fd("Error\nMissing required header(s)", 2);
    clean_exit(&game);
}
```

---

**Trap 1.5: File with only whitespace / blank lines**
```
   
		
  
```

Every line is whitespace. GNL returns them normally — your parser must skip blank-only lines during header parsing without treating them as a map trigger:
```c
int is_blank_line(char *line)
{
    int i;

    i = 0;
    while (line[i])
    {
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
            return (0);
        i++;
    }
    return (1);
}
```

---

### Category 2: Header Section Traps

**Trap 2.1: Missing headers — partial set**
```
NO ./north.xpm
SO ./south.xpm
# Missing WE, EA, F, C
1111
1001
1111
```

Track each header with a boolean flag. Only proceed after all 6 are confirmed set.

**Trap 2.2: Duplicate headers**
```
NO ./north.xpm
NO ./other_north.xpm    ← Second NO
SO ./south.xpm
...
```

The second `NO` must be an error, not a silent overwrite that leaks the first string:
```c
if (game->tex_path[NORTH] != NULL)
    return (parse_error(&game, "Duplicate NO identifier"));
game->tex_path[NORTH] = ft_strdup(value);
```

**Trap 2.3: Header with missing value**
```
NO
SO ./south.xpm
```

`NO` with nothing after it — `ft_split(line, ' ')` returns a 1-element array. Dereferencing `parts[1]` is undefined behavior.

```c
// After splitting:
if (!parts[1] || parts[1][0] == '\0')
    return (parse_error(&game, "NO requires a path argument"));
```

**Trap 2.4: Valid header identifier with garbage value**
```
NO not_a_real_path.xpm
```

The path parses fine but `mlx_xpm_file_to_image` returns `NULL`. This must be caught after MLX is initialized:
```c
tex.img = mlx_xpm_file_to_image(game.mlx, path, &w, &h);
if (!tex.img)
{
    ft_putstr_fd("Error\nCannot load texture: ", 2);
    ft_putendl_fd(path, 2);
    clean_exit(&game);
}
```

**Trap 2.5: RGB value out of range**
```
F 256,0,0       ← 256 is invalid (max 255)
F -1,0,0        ← Negative value
F 0,0,99999     ← Way out of range
F 255,255,      ← Trailing comma, empty third component
F abc,0,0       ← Non-numeric
```

```c
// After ft_split on ',':
int r = ft_atoi(parts[0]);
if (r < 0 || r > 255 || !ft_str_isdigit(parts[0]))
    return (parse_error(&game, "RGB component out of range"));
```

---

### Category 3: Map Section Traps

**Trap 3.1: Headers appearing after map lines**
```
1111
1001
1111
NO ./north.xpm   ← Header AFTER the map — must be an error
```

Once `map_started = 1`, any line starting with a valid header identifier is treated as an unknown map character and should trigger the invalid character check.

**Trap 3.2: Map with no player spawn**
```
11111
10001
10001
11111
```

Flood fill cannot start. Must be caught during spawn scan:
```c
if (player_count == 0)
    return (parse_error(&game, "No player spawn found (need N, S, E, or W)"));
```

**Trap 3.3: Map with multiple player spawns**
```
11111
1N0S1   ← Both N and S in same map
11111
```

```c
if (player_count > 1)
    return (parse_error(&game, "Multiple spawn characters found — only 1 allowed"));
```

**Trap 3.4: Invalid characters in map**
```
11211   ← '2' is not a valid map character
1A001   ← 'A' is not a direction character (only N,S,E,W are)
1!001
```

Character allowlist: `{'0', '1', ' ', '\t', 'N', 'S', 'E', 'W'}`. Everything else is an error.

**Trap 3.5: Map not enclosed — flood fill failure**
```
11111
10001
10000   ← Open right edge
11111
```

Flood fill leaks into void, returns failure. This is the most common student error during evaluation.

**Trap 3.6: Completely empty map section**
```
NO ./north.xpm
SO ./south.xpm
WE ./west.xpm
EA ./east.xpm
F 100,100,100
C 50,50,50

[just blank lines after headers, no map]
```

After the parse loop, `map.height == 0`. Must be guarded:
```c
if (game.map.height == 0)
    return (parse_error(&game, "Map section is empty"));
```

**Trap 3.7: Map width = 1 or height = 1**
```
1
```
A 1-line or 1-column map cannot be enclosed (it is just a wall segment). The flood fill will immediately fail, but a specific error is more helpful:
```c
if (game.map.width < 3 || game.map.height < 3)
    return (parse_error(&game, "Map too small to be enclosed"));
```

---

### Category 4: GNL Static Buffer Traps

**Trap 4.1: Breaking out of GNL loop early without flushing**

As covered in `01-file-io-gnl.md`, if you `break` or `return` from your GNL loop before reaching EOF, the static buffer inside GNL leaks:

```c
/* WRONG — breaks immediately on bad line without flushing GNL */
while ((line = get_next_line(fd)) != NULL)
{
    if (!is_valid_line(line))
    {
        free(line);
        break; // 🚨 GNL static buffer still has data allocated!
    }
    free(line);
}
```

✅ **Always flush GNL on early exit:**
```c
void gnl_flush(int fd)
{
    char *dump;

    dump = get_next_line(fd);
    while (dump != NULL)
    {
        free(dump);
        dump = get_next_line(fd);
    }
}

// On error:
free(line);
gnl_flush(fd);
close(fd);
clean_exit(&game);
```

---

## Quick Reference: Complete Error Matrix

| Input | Expected Output | Common Bug |
|-------|----------------|------------|
| `./cub3d` (no args) | `Error\nUsage: ./cub3d <map.cub>` | argc check only, no helpful message |
| `./cub3d map.txt` | `Error\nFile must have .cub extension` | Not checking extension suffix |
| `./cub3d /etc` | `Error\nPath is a directory` | Not using `stat()`, crashes on read |
| `./cub3d badperms.cub` | `Error\nPermission denied` | Treating all open failures as "not found" |
| Empty `.cub` file | `Error\nMissing required headers` | Segfault — no guard for empty parse loop |
| Duplicate `NO` header | `Error\nDuplicate identifier: NO` | Silent overwrite + memory leak |
| `F 256,0,0` | `Error\nRGB value out of range` | `atoi` overflow not checked |
| Map with 0 spawns | `Error\nNo player spawn found` | Flood fill crash on NULL position |
| Map with 2 spawns | `Error\nMultiple spawn characters` | Only last direction stored |
| Open right edge | `Error\nMap is not enclosed` | Missing flood fill implementation |
| Header after map start | `Error\nInvalid character in map` | Not tracking `map_started` flag |
