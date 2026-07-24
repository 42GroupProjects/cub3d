/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 16:36:33 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/24 18:48:27 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	calculate_wall_x(t_cub *c, t_ray *r)
{
	if (r->side == 0)
		r->wall_x = c->player->y + r->perp_wall_dist * r->ray_dir_y;
	else
		r->wall_x = c->player->x + r->perp_wall_dist * r->ray_dir_x;
	r->wall_x -= floor(r->wall_x);
}

void	calculate_tx_x(t_texture *tx, t_ray *r)
{
	r->tx_x = (int)(r->wall_x * tx->width);
	if (r->side == 0 && r->ray_dir_y > 0)
		r->tx_x = tx->width - r->tx_x - 1;
	if (r->side == 1 && r->ray_dir_y < 0)
		r->tx_x = tx->width - r->tx_x - 1;
}

static int	get_texture_pixel(t_texture *t, int x, int y)
{
	char	*pixel;

	pixel = t->addr + y * t->line_len + x * (t->bpp / 8);
	return (*(unsigned int *)pixel);
}

void	draw_textured_line(t_cub *c, t_texture *t, t_ray *r, int x)
{
	int		y;
	int		tx_y;
	int		color;
	double	step;
	double	tx_pos;

	step = 1.0 * t->height / r->line_height;
	tx_pos = (r->draw_start - HEIGHT / 2 + r->line_height / 2) * step;
	y = r->draw_start;
	while (y <= r->draw_end)
	{
		tx_y = (int)tx_pos;
		if (tx_y < 0)
			tx_y = 0;
		if (tx_y >= t->height)
			tx_y = t->height - 1;
		color = get_texture_pixel(t, r->tx_x, tx_y);
		put_pixel(c, x, y, color);
		tx_pos += step;
		y++;
	}
}
