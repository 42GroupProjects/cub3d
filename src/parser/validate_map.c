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

/* validate_map_layout() now lives in flood_fill.c (it calls the two
 * helpers above plus the DFS enclosure check). */
