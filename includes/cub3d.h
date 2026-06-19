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
	char	*no_tex;
	char	*so_tex;
	char	*we_tex;
	char	*ea_tex;
	int		floor[3];
	int		ceiling[3];
	int		has_floor;
	int		has_ceiling;
	char	**map;
	int		width;
	int		height;
}	t_game;

/* orchestrator */
int		parse_config(t_game *game, char *file);

/* file / header */
int		validate_file(char *file);
int		parse_header(t_game *game, char **lines, int *map_start);
int		set_texture(char **dst, char *value);
int		set_color(int *rgb, char *value, int *flag);

/* map */
char	**append_line(char **map, char *line);
int		extract_map(t_game *game, char **lines, int map_start);
int		normalize_map(t_game *game);
int		validate_map_layout(t_game *game);
int		validate_characters(char **map);
int		validate_player_count(char **map);

/* output / cleanup */
void	print_config(t_game *game);
void	print_map(char **map);
void	free_config(t_game *game);
void	free_map(char **map);

/* utils */
int		parse_error(char *msg);
char	*skip_spaces(char *s);
int		match_id(char *line, char *id);
int		is_blank(char *line);

#endif
