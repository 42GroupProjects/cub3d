/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_full.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 20:30:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/21 20:45:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_dim_screen(t_cub *cub)
{
	int	x;
	int	y;

	cub->mm_alpha = MM_FULL_ALPHA;
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			mm_put_pixel_blend(cub, x, y, 0x000000);
			x++;
		}
		y++;
	}
}

static void	fill_tile(t_cub *cub, int ox, int oy, int color)
{
	int	px;
	int	py;
	int	tile;

	tile = mm_full_tile_size(cub);
	py = 0;
	while (py < tile)
	{
		px = 0;
		while (px < tile)
		{
			mm_put_pixel_blend(cub, ox + px, oy + py, color);
			px++;
		}
		py++;
	}
}
