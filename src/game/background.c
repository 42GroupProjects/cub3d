/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:40:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:40:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	rgb_to_int(int rgb[3])
{
	return ((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
}

static int	shade_bg(int color, double t)
{
	double	factor;
	int		r;
	int		g;
	int		b;

	factor = 1.0 - t * BG_GRAD;
	if (factor < BG_GRAD_MIN)
		factor = BG_GRAD_MIN;
	r = (int)(((color >> 16) & 0xFF) * factor);
	g = (int)(((color >> 8) & 0xFF) * factor);
	b = (int)((color & 0xFF) * factor);
	return ((r << 16) | (g << 8) | b);
}

static int	row_color(t_cub *cub, int y)
{
	int		base;
	double	t;

	if (y < HEIGHT / 2)
	{
		base = rgb_to_int(cub->config->ceiling);
		t = (double)y / (HEIGHT / 2);
	}
	else
	{
		base = rgb_to_int(cub->config->floor);
		t = (double)(HEIGHT - 1 - y) / (HEIGHT / 2);
	}
	return (shade_bg(base, t));
}

void	draw_background(t_cub *cub)
{
	int	y;
	int	x;
	int	color;

	y = 0;
	while (y < HEIGHT)
	{
		color = row_color(cub, y);
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(cub, x, y, color);
			x++;
		}
		y++;
	}
}
