/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:07:48 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/30 19:24:56 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//void	cast_single_ray(t_cub *cub)
//{
//	double	x;
//	double	y;
//	double	dir_x;
//	double	dir_y;
//
//	dir_x = cub->player->dir_x;
//	dir_y = cub->player->dir_y;
//	x = cub->player->x;
//	y = cub->player->y;
//	while (cub->config->map[(int)y][(int)x] != '1')
//	{
//		put_pixel(cub, x * TS, y * TS, 0x00FF00);
//		x += dir_x * STEP;
//		y += dir_y * STEP;
//	}
//	put_pixel(cub, x * TS, y * TS, 0xFF0000);
//}

void	init_ray(t_cub *c, t_ray *r, int x)
{
	r->camera_x = 2.0 * x / WIDTH - 1.0;
	r->ray_dir_x = c->player->dir_x + c->player->plane_x * r->camera_x;
	r->ray_dir_y = c->player->dir_y + c->player->plane_y * r->camera_y;
	r->map_x = (int)c->player->x;
	r->map_y = (int)c->player->y;
	r->delta_dist_x = fabs(1.0 / r->ray_dir_x);
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
		r->side_dist_y = (r->map_y + 1.0 - c->player->y)* r->delta_dist_y;
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
		if (c->config->map[r->map_y][r->map_x] == '1')
			r->hit = 1;
	}
}

void	cast_ray(t_cub *c, int x)
{
	t_ray r;

	init_ray(c, &r, x);
	calculate_step(c, &r);
	perform_dda(c, &r);
}
