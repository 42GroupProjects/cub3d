#include "cub3d.h"

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