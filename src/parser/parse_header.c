#include "cub3d.h"

/* Is this line one of the 6 recognized identifiers? */
static int	is_header(char *line)
{
	if (match_id(line, "NO") || match_id(line, "SO")
		|| match_id(line, "WE") || match_id(line, "EA"))
		return (1);
	if (match_id(line, "F") || match_id(line, "C"))
		return (1);
	return (0);
}

/* Route one header line to the right setter (texture or color). */
static int	parse_header_line(t_game *game, char *line)
{
	if (match_id(line, "NO"))
		return (set_texture(&game->no_tex, line + 2));
	if (match_id(line, "SO"))
		return (set_texture(&game->so_tex, line + 2));
	if (match_id(line, "WE"))
		return (set_texture(&game->we_tex, line + 2));
	if (match_id(line, "EA"))
		return (set_texture(&game->ea_tex, line + 2));
	if (match_id(line, "F"))
		return (set_color(game->floor, line + 1, &game->has_floor));
	return (set_color(game->ceiling, line + 1, &game->has_ceiling));
}

/* All 4 textures and both colors must be present before the map. */
static int	check_header_complete(t_game *game)
{
	if (!game->no_tex || !game->so_tex || !game->we_tex || !game->ea_tex)
		return (parse_error(ERR_MISSING_TEX));
	if (!game->has_floor || !game->has_ceiling)
		return (parse_error(ERR_MISSING_COLOR));
	return (1);
}

/* Consume header lines (skipping blanks) until the first map line.
 * *map_start receives the index where the map grid begins.               */
int	parse_header(t_game *game, char **lines, int *map_start)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		if (is_blank(lines[i]))
			i++;
		else if (is_header(lines[i]))
		{
			if (!parse_header_line(game, lines[i]))
				return (0);
			i++;
		}
		else
			break ;
	}
	*map_start = i;
	return (check_header_complete(game));
}
