#include "cub3d.h"

char **append_line(char **map, char *line)
{
	char **new_map;
	int i;
	int size;

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

void print_map(char **map)
{
	int i;

	i = 0;
	while (map[i])
	{
		printf("%s", map[i]);
		i++;
	}
}