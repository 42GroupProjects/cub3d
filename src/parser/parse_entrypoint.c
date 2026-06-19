#include "cub3d.h"

/**
 * Append a duplicate of `line` to a NULL-terminated array. On strdup failure
 * the new array is freed (the old rows still belong to `map`, so the caller
 * can free `map`) and NULL is returned. NULL on any allocation failure.
 */
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
	if (!new_map[i])
		return (free(new_map), NULL);
	new_map[i + 1] = NULL;
	free(map);
	return (new_map);
}

/** Remove a single trailing '\n' so width/padding math stays clean. */
static void	rstrip_newline(char *s)
{
	int	len;

	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		s[len - 1] = '\0';
}

/**
 * Read every line of `file` into a NULL-terminated array (newlines removed).
 * Returns NULL on open failure or any allocation failure; partial work is
 * freed before returning.
 */
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
			return (free_strarr(&lines), close(fd), NULL);
		lines = tmp;
		line = get_next_line(fd);
	}
	close(fd);
	return (lines);
}

/**
 * Full parse pipeline: validate file, load lines, parse header, extract +
 * normalize map, then the (stubbed) layout check. Frees `lines` on every
 * path. Returns SUCCESS, FAILURE (bad input) or OOM (allocation failure).
 */
int	parse_config(t_game *game, char *file)
{
	char	**lines;
	int		map_start;
	int		status;

	status = validate_file(file);
	if (status != SUCCESS)
		return (status);
	lines = load_lines(file);
	if (!lines)
		return (parse_error(ERR_FILE_READ));
	status = parse_header(game, lines, &map_start);
	if (status != SUCCESS)
		return (free_strarr(&lines), status);
	status = extract_map(game, lines, map_start);
	free_strarr(&lines);
	if (status != SUCCESS)
		return (status);
	status = normalize_map(game);
	if (status != SUCCESS)
		return (status);
	return (validate_map_layout(game));
}
