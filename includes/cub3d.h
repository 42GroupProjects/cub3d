#ifndef CUB3D_H
#define CUB3D_H

#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../libft/libft.h"

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
