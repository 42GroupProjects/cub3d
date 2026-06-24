#include "cub3d.h"

/** Print the command-line usage to stderr. */
static void	usage(void)
{
	ft_putstr_fd("Usage: ./cub3d <map.cub>\n", 2);
}

/**
 * Entry point. Validates argc, runs the parser, prints the parsed config.
 * free_config runs on BOTH the success and the error path, so a failure
 * partway through parsing never leaks the work already allocated.
 */
int	main(int argc, char **argv)
{
	t_game	game;
	t_cub	cub;

	ft_bzero(&game, sizeof(t_game));
	if (argc != 2)
		return (usage(), 1);
	if (parse_config(&game, argv[1]) != SUCCESS)
		return (free_config(&game), 1);
	print_config(&game);
	ft_bzero(&cub, sizeof(t_cub));
	if (init_game(&cub, &game) != SUCCESS)
		return (free_config(&game), 1);	//need to free &cub in both ways
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_loop(cub.mlx);
	free_cub_struct(&cub);
	free_config(&game);
	return (0);
}
