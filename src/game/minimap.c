/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 18:44:57 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/25 20:21:35 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_circle(t_cub *c, int cx, int cy, int radius)
{
	int	x;
	int	y;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= radius * radius)
				put_pixel(c, cx + x, cy + y, 0xFF00FF);
			x++;
		}
		y++;
	}
}

static void	draw_square(t_cub *cub, int start_x, int start_y, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < TS)
	{
		x = 0;
		while (x < TS)
		{
			put_pixel(cub, start_x + x, start_y + y, color);
			x++;
		}
		y++;
	}
}

static void	draw_minimap_tile(t_cub *c, int map_x, int map_y, int x, int y)
{
	int	color;

	if (map_x < 0 || map_y < 0)
		return ;
	if (c->config->map[map_y][map_x] == '1')
		color = 0xFFFFFF;
	else
		color = 0x000000;
	draw_square(c, MM_X + x * TS, MM_Y + y * TS, color);
}

void	draw_minimap(t_cub *c)
{
	int	start_x;
	int	start_y;
	int	x;
	int	y;

	start_x = (int)c->player->x - 7;
	start_y = (int)c->player->y - 7;

	y = 0;
	while (y < MM_T)
	{
		x = 0;
		while (x < MM_T)
		{
			draw_minimap_tile(c, start_x + x, start_y + y, x, y);
			x++;
		}
		y++;
	}
}

void	draw_player(t_cub *c)
{
	int	x;
	int	y;

	x = MM_X + 7 * TS;
	y = MM_Y + 7 * TS;
	draw_circle(c, x, y, 4);
}
