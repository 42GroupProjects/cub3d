# Theory: Algorithmic Flood Fill

The 42 `cub3D` subject dictates that the map must be completely surrounded by walls.
If there are any holes where the map touches an empty exterior space, or if the player spawns outside of the enclosed area, the parser must cleanly reject the file and safely exit the program.

## 0. Why NOT Rim Checking? (The Obvious Wrong Answer)

The instinctive approach: scan the first row, last row, first column, and last column for `'0'` tiles. If any edge cell is walkable, the map is open.

### ❌ The Perimeter Scan Implementation

```c
/* NAIVE PERIMETER SCAN — Fails on complex maps */
int    check_map_closed_naive(char **map, int height, int width)
{
    int    i;

    i = 0;
    while (i < width)
    {
        if (map[0][i] == '0' || map[height - 1][i] == '0')
            return (0); // Top or bottom row exposed
        i++;
    }
    i = 0;
    while (i < height)
    {
        if (map[i][0] == '0' || map[i][width - 1] == '0')
            return (0); // Left or right column exposed
        i++;
    }
    return (1); // Passes!
}
```

### Why It Fails Silently

Consider this map — the perimeter scan passes it as "valid":
```
111111111
100001111
100000   
100000111    ← The '0' cells here escape to spaces on the right
111111111
```

The outer rim of this map is all `'1'` walls. The naive scan reports **VALID**. But the internal floor tiles on row 3 bleed into the whitespace dead zone on the right side of the row. A player spawning there would walk into undefined memory territory.

The perimeter scan only checks the rectangle boundary — it is completely blind to interior corridors that tunnel out through shorter rows.

### The Second Failure Mode: Irregular Map Shapes

```
    11111
    10001
11111000111
10000000001     ← This is valid
11111000111
    10001
    11111
```

The leftmost column of this "cross" shaped map is `'1'` at row 0 only — rows 2-4 are off the left edge entirely (the row is shorter). The perimeter scan would try `map[2][0]` and read an out-of-bounds or empty character, producing a false safety signal.

### ✅ Flood Fill: Proves Enclosure From The Inside

Instead of checking the boundary from the outside, Flood Fill starts from the player's known position *inside* the map and attempts to escape. If it successfully reaches the edge or a space character, the map has a hole. If it terminates having only hit walls, the map is provably enclosed.

To validate unpredictable geometry, we use a Depth-First Search (DFS) algorithm called **Flood Fill**.


## The Flood Fill Principle
1. Iterate over your map.
2. The instant you find the Player character ('N', 'S', 'E', or 'W'), trigger the `flood_fill(int x, int y)` function.
3. The function checks its current tile. If the tile is a boundary violation (like an outer space ` ` or the absolute edge of the array), the map is INVALID.
4. If the tile is a wall `'1'`, it returns instantly (safe boundary reached).
5. If the tile is a valid floor `'0'` or player tile, it changes the character to a visited marker (e.g. `'V'`).
6. It then recursively calls `flood_fill()` on its North, South, East, and West neighbors.

```mermaid
flowchart TD
    Spawn([Player spawns at X, Y]) --> CallFlood[flood_fill(X, Y)]
    CallFlood --> Eval{Evaluate Tile}
    
    Eval -- "Tile == '1'\nor Tile == 'V'" --> ReturnSafe[Return SUCCESS]
    Eval -- "Tile == Out-of-Bounds\nor Tile == ' '" --> ReturnFail[Return MAP INVALID!]
    
    Eval -- "Tile == '0'\nor Tile == 'N/S/E/W'" --> MarkTile[Tile = 'V']
    
    MarkTile --> RecN[flood_fill(X, Y-1)]
    MarkTile --> RecS[flood_fill(X, Y+1)]
    MarkTile --> RecE[flood_fill(X+1, Y)]
    MarkTile --> RecW[flood_fill(X-1, Y)]
    
    RecN --> Comb[Combine results]
    RecS --> Comb
    RecE --> Comb
    RecW --> Comb
    
    Comb --> ReturnResult[Return SUCCESS if all 4 are safe]
```

## Defensive Parsing

Because Flood Fill intentionally mutates the map array (filling it with `'V'`), you must run this against a **duplicate** of your map struct. Keep your original `char **` intact for actual rendering, and free the duplicate array right after validation concludes.

```c
int flood_fill(char **map, int x, int y, int max_x, int max_y)
{
    // 1. Boundary / Leak conditions
    if (x < 0 || y < 0 || x >= max_x || y >= max_y)
        return (0); // Error: touched the edge of the known universe!
    if (map[y][x] == ' ' || map[y][x] == '\t')
        return (0); // Error: leaked into outer whitespace!
        
    // 2. Safe boundaries
    if (map[y][x] == '1' || map[y][x] == 'V')
        return (1); // Safe: wall or already visited
        
    // 3. Mark as visited
    map[y][x] = 'V';
    
    // 4. Branch in 4 directions
    if (!flood_fill(map, x, y - 1, max_x, max_y)) return (0); // North
    if (!flood_fill(map, x, y + 1, max_x, max_y)) return (0); // South
    if (!flood_fill(map, x - 1, y, max_x, max_y)) return (0); // West
    if (!flood_fill(map, x + 1, y, max_x, max_y)) return (0); // East
    
    return (1);
}
```
