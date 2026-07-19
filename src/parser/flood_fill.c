/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:22 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:54:32 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

static int	flood_fill(char **map, int x, int y, t_game *game)
{
	if (x < 0 || y < 0 || x >= game->width || y >= game->height)
		return (FALSE);
	if (map[y][x] == ' ')
		return (FALSE);
	if (map[y][x] == '1' || map[y][x] == 'V')
		return (TRUE);
	map[y][x] = 'V';
	if (!flood_fill(map, x, y - 1, game)
		|| !flood_fill(map, x, y + 1, game)
		|| !flood_fill(map, x - 1, y, game)
		|| !flood_fill(map, x + 1, y, game))
		return (FALSE);
	return (TRUE);
}

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
	{
		free_strarr(&copy);
		return (parse_error(ERR_MAP_NOT_CLOSED));
	}
	free_strarr(&copy);
	return (SUCCESS);
}
