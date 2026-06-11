/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 20:31:40 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/10 20:41:04 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	init_images(t_cub *cub)
{
	cub->wall.i = mlx_xpm_file_to_image(cub->mlx, "./img/wall.xpm",
				&cub->wall.width, &cub->wall.height);
	if (!cub->wall.i)
	{
		write(1, "FAILED TO LOAD WALL IMG\n", 26);
		return (1);
	}
	cub->floor.i = mlx_xpm_file_to_image(cub->mlx, "./img/floor.xpm",
				&cub->wall.floor, &cub->floor.height);
	if (!cub->floor.i)
	{
		write(1, "FAILED TO LOAD FLOOR IMG\n", 27);
		return (1);
	}
	return (0);
}
