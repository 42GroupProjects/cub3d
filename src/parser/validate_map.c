#include "cub3d.h"

/* Helper: only 0/1/space and the spawn chars N/S/E/W are legal cells.
 * Kept ready for you to call from validate_map_layout when you build it.   */
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
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

/* Helper: there must be exactly one spawn (N/S/E/W). */
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

/* STUB — the map is treated as valid for now.
 *
 * This is YOUR hook. Implement the layout checks here yourself, e.g.:
 *   - if (!validate_characters(game->map))   reject bad chars
 *   - if (!validate_player_count(game->map)) reject 0 or 2+ spawns
 *   - flood fill / closure check to ensure the map is sealed by walls
 *
 * The grid is already normalized (rectangular, padded with spaces), so
 * game->map[y][x] is safe for 0 <= y < height, 0 <= x < width.            */
int	validate_map_layout(t_game *game)
{
	(void)game;
	return (1);
}
