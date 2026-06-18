# 04 — Floor/Ceiling Color Parsing

The configuration file specifies floor and ceiling colors using a comma-separated format: `F 220,100,0`. Extracting these safely and converting them into a format MLX can use is a frequent stumbling block.

## 1. String Splitting and Validation
You cannot assume the color string is perfectly formatted. It might contain spaces (`220, 100, 0`), multiple commas (`220,,0`), or alpha characters (`220A, 10, 5`).

```c
// Pseudo-validation step
char **rgb = ft_split(color_string, ',');
if (get_array_length(rgb) != 3)
    return (throw_error("Invalid RGB format"));

for (int i = 0; i < 3; i++)
{
    if (!is_strictly_numeric(rgb[i]))
        return (throw_error("RGB contains non-numeric characters"));
        
    int val = ft_atoi(rgb[i]);
    if (val < 0 || val > 255)
        return (throw_error("RGB values must be between 0 and 255"));
}
```

## 2. Bitwise Shifting to Hexadecimal
MLX expects colors represented as a single `int` value. A 32-bit integer has 4 bytes, perfect for holding A, R, G, and B data. 

Instead of multiplying, we use bitwise left-shifts (`<<`) to push the red, green, and blue values into their correct byte slots inside the integer.

```c
// Create a 32-bit integer: [ Alpha (0) ][ Red ][ Green ][ Blue ]
int create_trgb(int t, int r, int g, int b)
{
    return (t << 24 | r << 16 | g << 8 | b);
}

// Usage after parsing the string:
game->floor_color = create_trgb(0, ft_atoi(rgb[0]), ft_atoi(rgb[1]), ft_atoi(rgb[2]));
```

## 3. Dealing with Edge Cases
Be careful when `ft_split` consumes consecutive delimiters. If the file has `C 255,,255`, standard `ft_split` might return an array of length 2, bypassing your length check and crashing on invalid array access. Write a robust splitter or check for consecutive commas before splitting!
