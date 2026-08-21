/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 17:46:38 by lwittwer          #+#    #+#             */
/*   Updated: 2026/08/21 21:43:03 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	put_pixel(t_cub *cub, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = cub->addr + (y * cub->line_len) + (x * (cub->bpp / 8));
	*(unsigned int *)dst = color;
}

static int	rgb_to_int(int rgb[3])
{
	return ((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
}

void	draw_background(t_cub *cub)
{
	int	y;
	int	x;
	int	color;

	color = rgb_to_int(cub->config->ceiling);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		if ((y == (HEIGHT / 2) - 1) && x == 0)
			color = rgb_to_int(cub->config->floor);
		while (x < WIDTH)
		{
			put_pixel(cub, x, y, color);
			x++;
		}
		y++;
	}
}

int	render(t_cub *cub)
{
	int	x;

	x = 0;
	draw_background(cub);
	while (x < WIDTH)
	{
		cast_ray(cub, x);
		x++;
	}
	if (cub->show_minimap)
	{
		draw_minimap(cub);
		draw_player(cub);
	}
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img, 0, 0);
	return (SUCCESS);
}
