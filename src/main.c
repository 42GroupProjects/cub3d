/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:42 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:52:58 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/** Print the command-line usage to stderr. */
static void	usage(void)
{
	ft_putstr_fd("Error\nUsage: ./cub3d <map.cub>\n", 2);
}

/**
 * Entry point. Validates argc, runs the parser, prints the parsed config.
 * Set CUB3D_PARSE_ONLY=1 to skip MLX (for map suites / valgrind).
 * Parse failures free partial state inside parse_config.
 */
int	main(int argc, char **argv)
{
	t_game	game;
	t_cub	cub;
	char	*parse_only;

	ft_bzero(&game, sizeof(t_game));
	if (argc != 2)
		return (usage(), 1);
	if (parse_config(&game, argv[1]) != SUCCESS)
		return (1);
	print_config(&game);
	parse_only = getenv("CUB3D_PARSE_ONLY");
	if (parse_only && parse_only[0] == '1' && parse_only[1] == '\0')
		return (free_config(&game), 0);
	ft_bzero(&cub, sizeof(t_cub));
	if (init_game(&cub, &game) != SUCCESS)
		return (free_config(&game), 1);
	mlx_hook(cub.win, 2, 1L << 0, handle_keypress, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_hook(cub.win, 17, 0, on_x, &cub);
	mlx_loop(cub.mlx);
	free_cub_struct(&cub);
	free_config(&game);
	return (0);
}
