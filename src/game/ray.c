/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:07:48 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/24 21:35:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	setup_ray(t_cub *c, t_ray *r, int x)
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

void	calculate_perp_wall_dist(t_cub *cub, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - cub->player->x
				+ (1 - ray->step_x) / 2.0) / ray->ray_dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - cub->player->y
				+ (1 - ray->step_y) / 2.0) / ray->ray_dir_y;
}

void	calculate_line_height(t_ray *r)
{
	r->line_height = HEIGHT / r->perp_wall_dist;
	r->draw_start = -r->line_height / 2 + HEIGHT / 2;
	r->draw_end = r->line_height / 2 + HEIGHT / 2;
	if (r->draw_start < 0)
		r->draw_start = 0;
	if (r->draw_end >= HEIGHT)
		r->draw_end = HEIGHT - 1;
}
