/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 20:30:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/21 20:45:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	mm_tile_color(char cell)
{
	if (cell == '1')
		return (MM_WALL);
	if (cell == '0' || cell == 'N' || cell == 'S'
		|| cell == 'E' || cell == 'W')
		return (MM_FLOOR);
	return (MM_VOID);
}

void	mm_put_pixel_blend(t_cub *cub, int x, int y, int color)
{
	unsigned int	*dst;
	int				src[3];
	int				dst_c[3];

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = (unsigned int *)(cub->addr + y * cub->line_len
			+ x * (cub->bpp / 8));
	src[0] = (color >> 16) & 0xFF;
	src[1] = (color >> 8) & 0xFF;
	src[2] = color & 0xFF;
	dst_c[0] = (*dst >> 16) & 0xFF;
	dst_c[1] = (*dst >> 8) & 0xFF;
	dst_c[2] = *dst & 0xFF;
	src[0] = (src[0] * cub->mm_alpha + dst_c[0]
			* (255 - cub->mm_alpha)) / 255;
	src[1] = (src[1] * cub->mm_alpha + dst_c[1]
			* (255 - cub->mm_alpha)) / 255;
	src[2] = (src[2] * cub->mm_alpha + dst_c[2]
			* (255 - cub->mm_alpha)) / 255;
	*dst = (src[0] << 16) | (src[1] << 8) | src[2];
}

void	mm_draw_dot(t_cub *cub, int cx, int cy, int size)
{
	int	dx;
	int	dy;

	dy = -size;
	while (dy <= size)
	{
		dx = -size;
		while (dx <= size)
		{
			put_pixel(cub, cx + dx, cy + dy, MM_PLAYER);
			dx++;
		}
		dy++;
	}
}

void	mm_draw_facing(t_cub *cub, int cx, int cy, int len)
{
	int	i;

	i = 1;
	while (i <= len)
	{
		put_pixel(cub, cx + (int)(cub->player->dir_x * i),
			cy + (int)(cub->player->dir_y * i), MM_DIR);
		i++;
	}
}
