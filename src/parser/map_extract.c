/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_extract.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:07 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 20:30:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

static int	pad_row(char **row, int width)
{
	char	*new_row;
	int		len;
	int		i;

	len = ft_strlen(*row);
	new_row = malloc(width + 1);
	if (!new_row)
		return (OOM);
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
	return (SUCCESS);
}

static int	dup_map_rows(t_game *game, char **lines, int start, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		game->map[i] = ft_strdup(lines[start + i]);
		if (!game->map[i])
		{
			free_strarr(&game->map);
			return (oom_error());
		}
		i++;
	}
	return (SUCCESS);
}

int	extract_map(t_game *game, char **lines, int map_start)
{
	int	count;

	count = 0;
	while (lines[map_start + count])
	{
		if (is_blank(lines[map_start + count]))
			return (parse_error(ERR_MAP_EMPTY_LINE));
		count++;
	}
	if (count == 0)
		return (parse_error(ERR_NO_MAP));
	game->map = ft_calloc(count + 1, sizeof(char *));
	if (!game->map)
		return (oom_error());
	if (dup_map_rows(game, lines, map_start, count) != SUCCESS)
		return (FAILURE);
	game->height = count;
	return (SUCCESS);
}

int	normalize_map(t_game *game)
{
	int	i;
	int	width;

	width = max_width(game->map);
	i = 0;
	while (game->map[i])
	{
		if (pad_row(&game->map[i], width) != SUCCESS)
			return (oom_error());
		i++;
	}
	game->width = width;
	return (SUCCESS);
}
