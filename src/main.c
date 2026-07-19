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
static void usage(void)
{
	ft_putstr_fd("Error\nUsage: ./cub3d <map.cub>\n", 2);
}

/**
 * Entry point. Validates argc, runs the parser, prints the parsed config.
 * free_config runs on BOTH the success and the error path, so a failure
 * partway through parsing never leaks the work already allocated.
 */
int main(int argc, char **argv)
{
	t_game game;
	t_cub cub;

	ft_bzero(&game, sizeof(t_game));
	if (argc != 2)
		return (usage(), 1);
	if (parse_config(&game, argv[1]) != SUCCESS)
		return (1);
	print_config(&game);
	ft_bzero(&cub, sizeof(t_cub));
	if (init_game(&cub, &game) != SUCCESS)
		return (free_config(&game), 1); /* cub is not yet initialized here */
	mlx_hook(cub.win, 2, 1L << 0, handle_keypress, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_hook(cub.win, 17, 0, on_x, &cub);
	mlx_loop(cub.mlx);
	free_cub_struct(&cub);
	free_config(&game);
	return (0);
}
