#include "cub3d.h"

static void	usage(void)
{
	ft_putstr_fd("Usage: ./cub3d <map.cub>\n", 2);
}

int	main(int argc, char **argv)
{
	t_game	game;

	ft_bzero(&game, sizeof(t_game));
	if (argc != 2)
	{
		usage();
		return (1);
	}
	if (!parse_config(&game, argv[1]))
		return (1);
	print_config(&game);
	free_config(&game);
	return (0);
}
