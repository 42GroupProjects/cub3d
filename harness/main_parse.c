/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parse.c                                       :+:      :+:    :+:   */
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

	ft_bzero(&game, sizeof(t_game));
	if (argc != 2)
		return (usage(), 1);
	if (parse_config(&game, argv[1]) != SUCCESS)
		return (1);
	return (free_config(&game), 0);
}
