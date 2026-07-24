/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:53:58 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/24 19:50:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	free_mlx(t_cub *cub)
{
	if (cub->mlx && cub->img)
	{
		mlx_destroy_image(cub->mlx, cub->img);
		cub->img = NULL;
	}
	if (cub->mlx && cub->win)
	{
		mlx_destroy_window(cub->mlx, cub->win);
		cub->win = NULL;
	}
	if (cub->mlx)
	{
		mlx_destroy_display(cub->mlx);
		free(cub->mlx);
		cub->mlx = NULL;
	}
	cub->addr = NULL;
}

void	free_cub_struct(t_cub *cub)
{
	if (!cub)
		return ;
	free_textures(cub);
	free_mlx(cub);
	cub->bpp = 0;
	cub->line_len = 0;
	cub->endian = 0;
	cub->config = NULL;
	if (cub->player)
	{
		free(cub->player);
		cub->player = NULL;
	}
	if (cub->ray)
	{
		free(cub->ray);
		cub->ray = NULL;
	}
}
