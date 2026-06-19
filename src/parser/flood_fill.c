#include "cub3d.h"

/**
 * Deep-copy the grid so flood fill can mutate it ('V' markers) without
 * touching the real map used for rendering. NULL on allocation failure.
 */
static char	**dup_map(char **map, int height)
{
	char	**copy;
	int		i;

	copy = ft_calloc(height + 1, sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < height)
	{
		copy[i] = ft_strdup(map[i]);
		if (!copy[i])
			return (free_strarr(&copy), NULL);
		i++;
	}
	return (copy);
}

/** Locate the single spawn cell (guaranteed to exist by player-count check). */
static void	find_player(char **map, int *px, int *py)
{
	int	x;
	int	y;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'N' || map[y][x] == 'S'
				|| map[y][x] == 'E' || map[y][x] == 'W')
			{
				*px = x;
				*py = y;
				return ;
			}
			x++;
		}
		y++;
	}
}

/**
 * DFS from the player. Returns TRUE if the reachable area is fully sealed,
 * FALSE the moment it leaks off the edge or into a void (' ') cell.
 * Visited floor is marked 'V'; walls ('1') and 'V' are safe stops.
 */
static int	flood_fill(char **map, int x, int y, t_game *game)
{
	if (x < 0 || y < 0 || x >= game->width || y >= game->height)
		return (FALSE);
	if (map[y][x] == ' ')
		return (FALSE);
	if (map[y][x] == '1' || map[y][x] == 'V')
		return (TRUE);
	map[y][x] = 'V';
	if (!flood_fill(map, x, y - 1, game) || !flood_fill(map, x, y + 1, game)
		|| !flood_fill(map, x - 1, y, game) || !flood_fill(map, x + 1, y, game))
		return (FALSE);
	return (TRUE);
}

/**
 * Full map-layout validation: legal characters, exactly one spawn, and the
 * spawn's reachable area sealed by walls (flood fill on a duplicate).
 * Returns SUCCESS, FAILURE (rejected map) or OOM (duplicate failed).
 */
int	validate_map_layout(t_game *game)
{
	char	**copy;
	int		px;
	int		py;

	if (!validate_characters(game->map))
		return (parse_error(ERR_MAP_INVALID));
	if (!validate_player_count(game->map))
		return (parse_error(ERR_MAP_NO_PLAYER));
	copy = dup_map(game->map, game->height);
	if (!copy)
		return (oom_error());
	find_player(copy, &px, &py);
	if (!flood_fill(copy, px, py, game))
		return (free_strarr(&copy), parse_error(ERR_MAP_NOT_CLOSED));
	free_strarr(&copy);
	return (SUCCESS);
}
