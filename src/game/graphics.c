/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 17:46:38 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/24 18:12:39 by lwittwer         ###   ########.fr       */
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

void	draw_square(t_cub *cub, int start_x, int start_y, int color)
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

void	draw_map(t_cub *cub)
{
	int		x;
	int		y;
	char	cell;

	y = 0;
	while (y < cub->config->height)
	{
		x = 0;
		while (cub->config->map[y][x])
		{
			cell = cub->config->map[y][x];
			if (cell == '1')
				draw_square(cub, x * TS, y * TS, 0xFFFFFF);
			else
				draw_square(cub, x * TS, y * TS, 0x000000);
			x++;
		}
		y++;
	}
}

void	draw_player(t_cub *cub)
{
	int	screen_x;
	int	screen_y;
	int	x;
	int	y;

	screen_x = cub->player->x * TS;
	screen_y = cub->player->y * TS;
	y = -2;
	while (y <= 2)
	{
		x = -2;
		while (x <= 2)
		{
			put_pixel(cub, screen_x + x, screen_y + y, 0xFF0000);
			x++;
		}
		y++;
	}
}

int	render(t_cub *cub)
{
	draw_map(cub);
	draw_player(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img, 0, 0);
	return (SUCCESS);
}
