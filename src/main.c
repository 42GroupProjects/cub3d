#include "cub3d.h"

static int validate_arguments(int argc, char **argv)
{
	char *ext;
	char *name;
	size_t len;

	if (argc != 2)
		error_exit(ERR_INVALID_ARG);
	ext = ".ber";
	name = ft_strrchr(argv[1], '/');
	if (name)
		name++;
	else
		name = argv[1];
	len = ft_strlen(name);
	if (len <= FILE_EXT_LENGTH || name[0] == '.')
		error_exit(ERR_INVALID_EXT);
	if (ft_strncmp(name, ext, FILE_EXT_LENGTH) != 0)
		if (ft_revstrncmp(name, ext, FILE_EXT_LENGTH) != 0)
			error_exit(ERR_INVALID_EXT);
	return (0);
}

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
