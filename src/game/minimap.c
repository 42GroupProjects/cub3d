/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 18:44:57 by lwittwer          #+#    #+#             */
/*   Updated: 2026/08/20 17:40:56 by lwittwer         ###   ########.fr       */
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

static void	draw_minimap_tile(t_cub *c, int map_x, int map_y, int x, int y)
{
	int	color;

	if (map_y < 0 || map_y >= c->config->height)
		color = BLACK;
	else if (map_x < 0 || map_x >= (int)ft_strlen(c->config->map[map_y]))
		color = BLACK;
	else if (c->config->map[map_y][map_x] == '1')
		color = BLACK;
	else if (c->config->map[map_y][map_x] == '0')
		color = MM_COLOR;
	else
		color = BLACK;
	draw_square(c, MM_X + x * MM_TS, MM_Y + y * MM_TS, color);
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

static void	draw_rect(t_cub *c, int x, int y, int width, int height)
{
	int	i;
	int	j;
	int	color;

	color = 0x000000;
	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			put_pixel(c, x + j, y + i, color);
			j++;
		}
		i++;
	}
}

void	draw_mm_frame(t_cub *c)
{
	int	x;
	int	y;
	int	w;
	int	h;

	x = MM_X;
	y = MM_Y;
	w = MM_SIZE;
	h = MM_SIZE;
	draw_rect(c, x - MM_BORDER, y - MM_BORDER, w + MM_BORDER * 2, MM_BORDER);
	draw_rect(c, x - MM_BORDER, y + h, w + MM_BORDER * 2, MM_BORDER);
	draw_rect(c, x - MM_BORDER, y, MM_BORDER, h);
	draw_rect(c, x + w, y, MM_BORDER, h);
}
