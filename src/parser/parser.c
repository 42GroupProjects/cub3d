#include "cub3d.h"

/* Append a duplicated line to a NULL-terminated array, freeing the old one. */
char	**append_line(char **map, char *line)
{
	char	**new_map;
	int		i;
	int		size;

	size = 0;
	while (map && map[size])
		size++;
	new_map = malloc(sizeof(char *) * (size + 2));
	if (!new_map)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_map[i] = map[i];
		i++;
	}
	new_map[i] = ft_strdup(line);
	new_map[i + 1] = NULL;
	free(map);
	return (new_map);
}

/* Remove a single trailing '\n' so width/padding math stays clean. */
static void	rstrip_newline(char *s)
{
	int	len;

	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		s[len - 1] = '\0';
}

/* Read every line of `file` into a NULL-terminated array (newlines removed). */
static char	**load_lines(char *file)
{
	int		fd;
	char	*line;
	char	**lines;
	char	**tmp;

	lines = NULL;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (NULL);
	line = get_next_line(fd);
	while (line)
	{
		rstrip_newline(line);
		tmp = append_line(lines, line);
		free(line);
		if (!tmp)
			return (free_map(lines), close(fd), NULL);
		lines = tmp;
		line = get_next_line(fd);
	}
	close(fd);
	return (lines);
}

/* Phase 1 = header (textures + colors), phase 2 = map grid + normalize.
 * Map layout validity is intentionally a stub (validate_map_layout).      */
int	parse_config(t_game *game, char *file)
{
	char	**lines;
	int		map_start;

	if (!validate_file(file))
		return (0);
	lines = load_lines(file);
	if (!lines)
		return (parse_error(ERR_FILE_READ));
	if (!parse_header(game, lines, &map_start))
		return (free_map(lines), 0);
	if (!extract_map(game, lines, map_start))
		return (free_map(lines), 0);
	free_map(lines);
	if (!normalize_map(game))
		return (0);
	if (!validate_map_layout(game))
		return (0);
	return (1);
}
