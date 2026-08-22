/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 22:30:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/08/22 22:30:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	usage(void)
{
	ft_putstr_fd("Error\nUsage: ./cub3D <map.cub>\n", 2);
}

int	main(int argc, char **argv)
{
	t_game	game;
	t_cub	cub;

	ft_bzero(&game, sizeof(t_game));
	ft_bzero(&cub, sizeof(t_cub));
	if (argc != 2)
		return (usage(), 1);
	if (parse_config(&game, argv[1]) != SUCCESS)
		return (1);
	if (init_game(&cub, &game) != SUCCESS)
		return (free_config(&game), 1);
	clean_exit(&cub, 0);
	return (0);
}
