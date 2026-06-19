#include "cub3d.h"

/* Longest row length across the whole grid. */
static int	max_width(char **map)
{
	int	i;
	int	len;
	int	max;

	max = 0;
	i = 0;
	while (map[i])
	{
		len = ft_strlen(map[i]);
		if (len > max)
			max = len;
		i++;
	}
	return (max);
}

/* Rebuild one row padded to `width` with trailing spaces. */
static int	pad_row(char **row, int width)
{
	char	*new_row;
	int		len;
	int		i;

	len = ft_strlen(*row);
	new_row = malloc(width + 1);
	if (!new_row)
		return (0);
	i = 0;
	while (i < len)
	{
		new_row[i] = (*row)[i];
		i++;
	}
	while (i < width)
		new_row[i++] = ' ';
	new_row[i] = '\0';
	free(*row);
	*row = new_row;
	return (1);
}

/* Copy the map block (lines[map_start..]) into game->map, set height. */
int	extract_map(t_game *game, char **lines, int map_start)
{
	int	count;
	int	i;

	count = 0;
	while (lines[map_start + count])
		count++;
	if (count == 0)
		return (parse_error(ERR_NO_MAP));
	game->map = ft_calloc(count + 1, sizeof(char *));
	if (!game->map)
		return (parse_error(ERR_MALLOC));
	i = 0;
	while (i < count)
	{
		game->map[i] = ft_strdup(lines[map_start + i]);
		if (!game->map[i])
			return (free_map(game->map), game->map = NULL,
				parse_error(ERR_MALLOC));
		i++;
	}
	game->height = count;
	return (1);
}

/* Normalize: pad all rows to the max width so grid[y][x] is always safe. */
int	normalize_map(t_game *game)
{
	int	i;
	int	width;

	width = max_width(game->map);
	i = 0;
	while (game->map[i])
	{
		if (!pad_row(&game->map[i], width))
			return (parse_error(ERR_MALLOC));
		i++;
	}
	game->width = width;
	return (1);
}
