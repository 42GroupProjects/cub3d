/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:42 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/08/22 23:30:13 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	usage(void)
{
	ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
}

static int	run_game(t_game *game)
{
	t_cub	cub;

	ft_bzero(&cub, sizeof(t_cub));
	if (init_game(&cub, game) != SUCCESS)
		return (FAILURE);
	mlx_hook(cub.win, 2, 1L << 0, handle_keypress, &cub);
	mlx_hook(cub.win, 3, 1L << 1, handle_keyrelease, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_hook(cub.win, 17, 0, on_x, &cub);
	mlx_loop(cub.mlx);
	clean_exit(&cub, 0);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_game	game;

	ft_bzero(&game, sizeof(t_game));
	if (argc != 2)
		return (usage(), 1);
	if (parse_config(&game, argv[1]) != SUCCESS)
		return (1);
	if (run_game(&game) != SUCCESS)
		return (free_config(&game), 1);
	return (0);
}
