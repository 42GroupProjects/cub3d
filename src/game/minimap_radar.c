/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_radar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 20:30:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/21 20:45:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_radar_pixel(t_cub *cub, int px, int py)
{
	double	wx;
	double	wy;
	int		mx;
	int		my;

	wx = cub->player->x + (px - MM_SIZE / 2) * MM_ZOOM;
	wy = cub->player->y + (py - MM_SIZE / 2) * MM_ZOOM;
	mx = (int)wx;
	my = (int)wy;
	if (mx >= 0 && my >= 0 && mx < cub->config->width
		&& my < cub->config->height)
		put_pixel(cub, MM_MARGIN + px, MM_MARGIN + py,
			mm_tile_color(cub->config->map[my][mx]));
	else
		put_pixel(cub, MM_MARGIN + px, MM_MARGIN + py, MM_VOID);
}

static void	draw_radar_tiles(t_cub *cub)
{
	int	px;
	int	py;

	py = 1;
	while (py < MM_SIZE - 1)
	{
		px = 1;
		while (px < MM_SIZE - 1)
		{
			draw_radar_pixel(cub, px, py);
			px++;
		}
		py++;
	}
}

static void	draw_radar_frame(t_cub *cub)
{
	int	i;

	i = 0;
	while (i < MM_SIZE)
	{
		put_pixel(cub, MM_MARGIN + i, MM_MARGIN, MM_BORDER);
		put_pixel(cub, MM_MARGIN + i, MM_MARGIN + MM_SIZE - 1, MM_BORDER);
		put_pixel(cub, MM_MARGIN, MM_MARGIN + i, MM_BORDER);
		put_pixel(cub, MM_MARGIN + MM_SIZE - 1, MM_MARGIN + i, MM_BORDER);
		i++;
	}
}
