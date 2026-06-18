#ifndef CUB3D_H
#define CUB3D_H

#include "mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
# include "../libft/libft.h"
# include "../libft/get_next_line.h"

char	**parse_map(char *filename);
void	print_map(char **map);
void	free_map(char **map);


#define SCREEN_W 800
#define SCREEN_H 600

typedef struct s_game
{
	char **map;
	int width;
	int height;
} t_game;

char **parse_map(char *filename);
void print_map(char **map);
void free_map(char **map);

#endif
