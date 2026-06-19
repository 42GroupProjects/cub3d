# 03 — XPM Loading & Structs

MiniLibX does not parse JPGs or PNGs natively out of the box in the 42 environment cleanly without massive overhead. It is strongly recommended to convert your wall textures to `.xpm` format.

## The Image Structure
We cannot just load a file and use an MLX drawing function. We need to extract the raw pixel data array from the loaded image so our math loop can sample it rapidly.

```c
typedef struct s_img {
    void  *img_ptr;      // The actual MLX image object
    char  *addr;         // The 1D character array containing A,R,G,B bytes
    int   bpp;           // Bits per pixel (usually 32)
    int   line_len;      // How many bytes represent one horizontal row
    int   endian;
    int   width;
    int   height;
} t_img;
```

## Loading the File
When parsing the map, you found a path `NO ./textures/north.xpm`.

```c
t_img tex_north;

// 1. Tell MLX to open the file and create an image object
tex_north.img_ptr = mlx_xpm_file_to_image(game->mlx, 
                                          "./textures/north.xpm", 
                                          &tex_north.width, 
                                          &tex_north.height);

if (!tex_north.img_ptr)
    return (throw_error("Failed to load north texture"));

// 2. Extract the raw memory address of the pixels
tex_north.addr = mlx_get_data_addr(tex_north.img_ptr, 
                                   &tex_north.bpp, 
                                   &tex_north.line_len, 
                                   &tex_north.endian);
```

## Destroying Textures
When closing the game, you must loop through all 4 texture structs and call `mlx_destroy_image(game->mlx, tex_north.img_ptr)`. If you do not do this securely, Valgrind will catch an X11/OpenGL memory leak.

> [!WARNING]
> Do NOT attempt to read `tex_north.addr[...` via a raw index loop without taking `tex_north.line_len` into account. An image might be 64 pixels wide, but OS padding can make `line_len` equal to `64 * 4 + padding`!
