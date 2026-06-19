#include "cub3d.h"

/* Free a NULL-terminated array of strings (map rows, split words, lines). */
void	free_map(char **map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}

/* Free everything owned by the parsed config. */
void	free_config(t_game *game)
{
	free(game->no_tex);
	free(game->so_tex);
	free(game->we_tex);
	free(game->ea_tex);
	if (game->map)
		free_map(game->map);
}
