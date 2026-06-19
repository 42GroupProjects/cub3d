#include "cub3d.h"

/** Print each row + newline (newlines were stripped during loading). */
void	print_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		printf("%s\n", map[i]);
		i++;
	}
}

/** Debug dump of the fully parsed config: textures, colors, then the grid. */
void	print_config(t_game *game)
{
	int	*f;
	int	*c;

	f = game->floor;
	c = game->ceiling;
	printf("NO %s\n", game->no_tex);
	printf("SO %s\n", game->so_tex);
	printf("WE %s\n", game->we_tex);
	printf("EA %s\n", game->ea_tex);
	printf("F %d,%d,%d\n", f[0], f[1], f[2]);
	printf("C %d,%d,%d\n", c[0], c[1], c[2]);
	printf("map %dx%d:\n", game->width, game->height);
	print_map(game->map);
}
