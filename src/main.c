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

static int	env_flag_on(char *name)
{
	char	*v;

	v = getenv(name);
	return (v && v[0] == '1' && v[1] == '\0');
}

/**
 * Entry point. Validates argc, runs the parser, prints the parsed config.
 * CUB3D_PARSE_ONLY=1 — parse then free_config (no MLX).
 * CUB3D_QUIT_AFTER_INIT=1 — init MLX then clean_exit (valgrind teardown).
 * Parse failures free partial state inside parse_config.
 */
int	main(int argc, char **argv)
{
	t_game	game;
	t_cub	cub;

	ft_bzero(&game, sizeof(t_game));
	if (argc != 2)
		return (usage(), 1);
	if (parse_config(&game, argv[1]) != SUCCESS)
		return (1);
	print_config(&game);
	if (env_flag_on("CUB3D_PARSE_ONLY"))
		return (free_config(&game), 0);
	ft_bzero(&cub, sizeof(t_cub));
	if (init_game(&cub, &game) != SUCCESS)
		return (free_config(&game), 1);
	if (env_flag_on("CUB3D_QUIT_AFTER_INIT"))
		clean_exit(&cub, 0);
	mlx_hook(cub.win, 2, 1L << 0, handle_keypress, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_hook(cub.win, 17, 0, on_x, &cub);
	mlx_loop(cub.mlx);
	clean_exit(&cub, 0);
	return (0);
}
