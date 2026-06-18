#ifndef CUB3D_H
#define CUB3D_H

#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "errors.h"
#include "../libft/libft.h"

/* ========================================================================== */
/*                          COLOR & GRAPHICS                                  */
/* ========================================================================== */

#define ONE_OVER_255 0.00392156862f
#define BYTES_PER_PIXEL 4 // RGBA = 4 bytes
#define BITS_IN_BYTE 8	  // For bit shift operations
#define PATH_BUFFER_SIZE 256
#define STRING_BUFFER_SIZE 32
#define FILE_EXT_LENGTH 4
#define GREEN "\033[0;32m"
#define CYAN "\033[1;96m"
#define RESET "\033[0m"

typedef struct s_game
{
	char **map;
	int width;
	int height;
} t_game;

char **parse_map(char *file);
void print_map(char **map);
void free_map(char **map);
int validate_characters(char **map);
int validate_player_count(char **map);
int validate_file(char *file);
char **append_line(char **map, char *line);

#endif
