#include "cub3d.h"

int main(int argc, char **argv)
{
	char **map;

	if (argc != 2)
	{
		printf("Usage: ./cub3D <map.cub>\n");
		return (1);
	}

	map = parse_map(argv[1]);
	if (!map)
	{
		printf("Error\nInvalid map\n");
		return (1);
	}

	print_map(map);
	free_map(map);

	return (0);
}
