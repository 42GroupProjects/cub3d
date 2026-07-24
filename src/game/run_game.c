/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:40:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:40:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef CUB3D_QUIT_AFTER_INIT

static void	after_init(t_cub *c)
{
	clean_exit(c, 0);
}

#else

static void	after_init(t_cub *c)
{
	(void)c;
}

#endif

int	run_game(t_game *game)
{
	t_cub	cub;

	ft_bzero(&cub, sizeof(t_cub));
	if (init_game(&cub, game) != SUCCESS)
		return (FAILURE);
	after_init(&cub);
	mlx_do_key_autorepeatoff(cub.mlx);
	mlx_hook(cub.win, 2, 1L << 0, handle_keypress, &cub);
	mlx_hook(cub.win, 3, 1L << 1, handle_keyrelease, &cub);
	mlx_hook(cub.win, 4, 1L << 2, handle_mouse_button, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_hook(cub.win, 17, 0, on_x, &cub);
	mlx_loop(cub.mlx);
	clean_exit(&cub, 0);
	return (SUCCESS);
}
