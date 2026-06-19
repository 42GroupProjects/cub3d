#include "cub3d.h"

/**
 * Helper for your future layout check: TRUE if every cell is one of
 * 0 1 N S E W space. Kept ready to call from validate_map_layout.
 */
int	validate_characters(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] != '0' && map[i][j] != '1'
				&& map[i][j] != 'N' && map[i][j] != 'S'
				&& map[i][j] != 'E' && map[i][j] != 'W'
				&& map[i][j] != ' ')
				return (FALSE);
			j++;
		}
		i++;
	}
	return (TRUE);
}

/** Helper: TRUE if there is exactly one spawn (N/S/E/W). */
int	validate_player_count(char **map)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'N' || map[i][j] == 'S'
				|| map[i][j] == 'E' || map[i][j] == 'W')
				count++;
			j++;
		}
		i++;
	}
	return (count == 1);
}

/**
 * STUB — the map is treated as valid for now (returns SUCCESS).
 *
 * This is YOUR hook. Implement the layout checks here, returning FAILURE
 * (via parse_error) when the map is rejected, e.g.:
 *   if (!validate_characters(game->map))
 *       return (parse_error(ERR_MAP_INVALID));
 *   if (!validate_player_count(game->map))
 *       return (parse_error(ERR_MAP_NO_PLAYER));
 *   flood fill / closure check to ensure the map is sealed by walls.
 *
 * The grid is already normalized (rectangular, space-padded), so
 * game->map[y][x] is safe for 0 <= y < height, 0 <= x < width.
 */
int	validate_map_layout(t_game *game)
{
	(void)game;
	return (SUCCESS);
}
