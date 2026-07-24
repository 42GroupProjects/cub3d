/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_entrypoint.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:16 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:52:59 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	{
		free(new_map);
		return (NULL);
	}
	new_map[i + 1] = NULL;
	free(map);
	return (new_map);
}

static void	rstrip_newline(char *s)
{
	int	len;

	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		s[len - 1] = '\0';
}

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
			return (get_next_line(-1), free_strarr(&lines),
				close(fd), NULL);
		lines = tmp;
		line = get_next_line(fd);
	}
	return (get_next_line(-1), close(fd), lines);
}

static int	cleanup_parse_failure(t_game *game, char **lines, int status)
{
	free_strarr(&lines);
	free_config(game);
	return (status);
}

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
		return (cleanup_parse_failure(game, lines, parse_error(ERR_FILE_READ)));
	status = parse_header(game, lines, &map_start);
	if (status != SUCCESS)
		return (cleanup_parse_failure(game, lines, status));
	status = extract_map(game, lines, map_start);
	if (status != SUCCESS)
		return (cleanup_parse_failure(game, lines, status));
	free_strarr(&lines);
	status = normalize_map(game);
	if (status != SUCCESS)
		return (cleanup_parse_failure(game, NULL, status));
	status = validate_map_layout(game);
	if (status != SUCCESS)
		return (cleanup_parse_failure(game, NULL, status));
	return (SUCCESS);
}
