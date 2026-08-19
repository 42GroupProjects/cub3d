/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:07:48 by lwittwer          #+#    #+#             */
/*   Updated: 2026/08/19 14:51:08 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_ray(t_cub *c, t_ray *r, int x)
{
	r->camera_x = 2.0 * x / WIDTH - 1.0;
	r->ray_dir_x = c->player->dir_x + c->player->plane_x * r->camera_x;
	r->ray_dir_y = c->player->dir_y + c->player->plane_y * r->camera_x;
	r->map_x = (int)c->player->x;
	r->map_y = (int)c->player->y;
	if (r->ray_dir_x == 0)
		r->delta_dist_x = INFINITY;
	else
		r->delta_dist_x = fabs(1.0 / r->ray_dir_x);
	if (r->ray_dir_y == 0)
		r->delta_dist_y = INFINITY;
	else
		r->delta_dist_y = fabs(1.0 / r->ray_dir_y);
	r->hit = 0;
}

void	calculate_step(t_cub *c, t_ray *r)
{
	if (r->ray_dir_x < 0)
	{
		r->step_x = -1;
		r->side_dist_x = (c->player->x - r->map_x) * r->delta_dist_x;
	}
	else
	{
		r->step_x = 1;
		r->side_dist_x = (r->map_x + 1.0 - c->player->x) * r->delta_dist_x;
	}
	if (r->ray_dir_y < 0)
	{
		r->step_y = -1;
		r->side_dist_y = (c->player->y - r->map_y) * r->delta_dist_y;
	}
	else
	{
		r->step_y = 1;
		r->side_dist_y = (r->map_y + 1.0 - c->player->y) * r->delta_dist_y;
	}
}

void	perform_dda(t_cub *c, t_ray *r)
{
	while (r->hit == 0)
	{
		if (r->side_dist_x < r->side_dist_y)
		{
			r->side_dist_x += r->delta_dist_x;
			r->map_x += r->step_x;
			r->side = 0;
		}
		else
		{
			r->side_dist_y += r->delta_dist_y;
			r->map_y += r->step_y;
			r->side = 1;
		}
		if (r->map_x < 0 || r->map_y < 0
			|| r->map_y >= c->config->height
			|| r->map_x >= c->config->width)
		{
			r->hit = 1;
			return ;
		}
		if (c->config->map[r->map_y][r->map_x] == '1')
			r->hit = 1;
	}
}

void	calculate_perp_wall_dist(t_cub *c, t_ray *r)
{
	if (r->side == 0)
	{
		r->perp_wall_dist = (r->map_x - c->player->x
				+ (1 - r->step_x) / 2.0) / r->ray_dir_x;
	}
	else
	{
		r->perp_wall_dist = (r->map_y - c->player->y
				+ (1 - r->step_y) / 2.0) / r->ray_dir_y;
	}
}

void	cast_ray(t_cub *c, int x)
{
	t_ray		r;
	t_texture	*tx;

	init_ray(c, &r, x);
	calculate_step(c, &r);
	perform_dda(c, &r);
	calculate_perp_wall_dist(c, &r);
	calculate_line_height(&r);
	calculate_wall_x(c, &r);
	tx = get_wall_texture(c, &r);
	calculate_tx_x(tx, &r);
	draw_textured_line(c, tx, &r, x);
}
