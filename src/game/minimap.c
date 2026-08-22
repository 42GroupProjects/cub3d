/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 18:44:57 by lwittwer          #+#    #+#             */
/*   Updated: 2026/08/22 19:59:01 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_square(t_cub *cub, int start_x, int start_y, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < MM_TS)
	{
		x = 0;
		while (x < MM_TS)
		{
			put_pixel(cub, start_x + x, start_y + y, color);
			x++;
		}
		y++;
	}
}

static void	draw_minimap_tile(t_cub *c, int x, int y)
{
	int	color;
	int	map_y;
	int	map_x;

	map_y = c->start_y + y;
	map_x = c->start_x + x;
	if (map_y < 0 || map_y >= c->config->height)
		color = BLACK;
	else if (map_x < 0 || map_x >= (int)ft_strlen(c->config->map[map_y]))
		color = BLACK;
	else if (c->config->map[map_y][map_x] == '1')
		color = BLACK;
	else if (c->config->map[map_y][map_x] == '0')
		color = MM_FLOOR_COLOR;
	else
		color = BLACK;
	draw_square(c, MM_X + x * MM_TS,
		(HEIGHT - (MM_T * MM_TS) - 20) + y * MM_TS, color);
}

void	draw_minimap(t_cub *c)
{
	int	x;
	int	y;

	c->start_x = (int)c->player->x - (MM_T / 2);
	c->start_y = (int)c->player->y - (MM_T / 2);
	y = 0;
	while (y < MM_T)
	{
		x = 0;
		while (x < MM_T)
		{
			draw_minimap_tile(c, x, y);
			x++;
		}
		y++;
	}
}

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
				put_pixel(c, cx + x, cy + y, MM_PLAYER_COLOR);
			x++;
		}
		y++;
	}
}

void	draw_player(t_cub *c)
{
	int	x;
	int	y;

	x = MM_X + (MM_T / 2) * MM_TS;
	y = (HEIGHT - (MM_T * MM_TS) - 20) + (MM_T / 2) * MM_TS;
	draw_circle(c, x, y, 4);
}
