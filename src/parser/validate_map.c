/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:22 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:41:35 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	validate_characters(char **map)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			c = map[i][j];
			if (!(c == '0' || c == '1' || c == 'N' || c == 'S'
					|| c == 'E' || c == 'W' || c == ' '))
				return (FALSE);
			j++;
		}
		i++;
	}
	return (TRUE);
}

static int	validate_player_count(char **map)
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

static int	validate_borders(t_game *game)
{
	int		x;
	int		y;
	char	c;

	y = 0;
	while (y < game->height)
	{
		x = 0;
		while (x < game->width)
		{
			c = game->map[y][x];
			if ((c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
				&& (x == 0 || y == 0
					|| x == game->width - 1
					|| y == game->height - 1))
				return (FALSE);
			x++;
		}
		y++;
	}
	return (TRUE);
}

int	validate_map_layout(t_game *game)
{
	int	closed;

	if (!validate_characters(game->map))
		return (parse_error(ERR_MAP_INVALID));
	if (!validate_player_count(game->map))
		return (parse_error(ERR_MAP_NO_PLAYER));
	if (!validate_borders(game))
		return (parse_error(ERR_MAP_NOT_CLOSED));
	closed = check_map_closed(game);
	if (closed == OOM)
		return (oom_error());
	if (!closed)
		return (parse_error(ERR_MAP_NOT_CLOSED));
	return (SUCCESS);
}
