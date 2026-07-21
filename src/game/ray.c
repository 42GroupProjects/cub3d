/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 17:07:48 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/21 18:51:17 by lwittwer         ###   ########.fr       */
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
	// TODO: use camera_x here. Thank Shawn for spotting the issues!!
	r->ray_dir_y = c->player->dir_y + c->player->plane_y * r->camera_x;
	r->map_x = (int)c->player->x;
	r->map_y = (int)c->player->y;
	// FIXME: guard ray_dir_* == 0 before divide (delta_dist → inf/nan)
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
		// FIXME: bounds-check map_x/map_y vs width/height before indexing (OOB crash on open maps)
		if (c->config->map[r->map_y][r->map_x] == '1')
			r->hit = 1;
	}
}

void	calculate_perp_wall_dist(t_cub *cub, t_ray *ray)
{
	if (ray->side == 0)
	{
		ray->perp_wall_dist = (ray->map_x - cub->player->x + (1 - ray->step_x) / 2.0) / ray->ray_dir_x;
	}
	else
	{	
		ray->perp_wall_dist = (ray->map_y - cub->player->y + (1 - ray->step_y) / 2.0) / ray->ray_dir_y;
	}
}

void	calculate_line_height(t_ray *r)
{
	r->line_height = HEIGHT / r->perp_wall_dist;
	r->draw_start = -r->line_height / 2 + HEIGHT / 2;
	r->draw_end = r->line_height / 2 + HEIGHT / 2;
	if (r->draw_start < 0)
		r->draw_start = 0;
	if (r->draw_end >= HEIGHT)
		r->draw_end = HEIGHT -1;
}

void	draw_vertical_line(t_cub *cub, t_ray *r, int x)
{
	int	y;
	int	color;

	y = r->draw_start;
	while (y <= r->draw_end)
	{
		color = get_wall_color(r);
		put_pixel(cub, x, y, color);
		y++;
	}
}

void	cast_ray(t_cub *c, int x)
{
	t_ray	r;

	init_ray(c, &r, x);
	calculate_step(c, &r);
	perform_dda(c, &r);
	calculate_perp_wall_dist(c, &r);
	calculate_line_height(&r);
	draw_vertical_line(c, &r, x);
}
