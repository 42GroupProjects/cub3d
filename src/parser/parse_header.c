/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:11:11 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:14:05 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_header(char *line)
{
	if (match_id(line, "NO") || match_id(line, "SO"))
		return (TRUE);
	if (match_id(line, "WE") || match_id(line, "EA"))
		return (TRUE);
	if (match_id(line, "F") || match_id(line, "C"))
		return (TRUE);
	return (FALSE);
}

static int	parse_header_line(t_game *game, char *line)
{
	if (match_id(line, "NO"))
		return (set_texture(&game->no_tex, line + TEX_ID_LEN));
	if (match_id(line, "SO"))
		return (set_texture(&game->so_tex, line + TEX_ID_LEN));
	if (match_id(line, "WE"))
		return (set_texture(&game->we_tex, line + TEX_ID_LEN));
	if (match_id(line, "EA"))
		return (set_texture(&game->ea_tex, line + TEX_ID_LEN));
	if (match_id(line, "F"))
		return (set_color(game->floor, line + 1, &game->has_floor));
	return (set_color(game->ceiling, line + 1, &game->has_ceiling));
}

static int	check_header_complete(t_game *game)
{
	if (!game->no_tex || !game->so_tex || !game->we_tex || !game->ea_tex)
		return (parse_error(ERR_MISSING_TEX));
	if (!game->has_floor || !game->has_ceiling)
		return (parse_error(ERR_MISSING_COLOR));
	return (SUCCESS);
}

int	parse_header(t_game *game, char **lines, int *map_start)
{
	int	i;
	int	status;

	i = 0;
	while (lines[i])
	{
		if (is_blank(lines[i]))
			i++;
		else if (is_header(lines[i]))
		{
			status = parse_header_line(game, lines[i]);
			if (status != SUCCESS)
				return (status);
			i++;
		}
		else
			break ;
	}
	*map_start = i;
	return (check_header_complete(game));
}
