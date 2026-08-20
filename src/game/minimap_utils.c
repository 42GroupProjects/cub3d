/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:46:51 by lwittwer          #+#    #+#             */
/*   Updated: 2026/08/20 15:58:21 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_line(t_cub *c, int x0, int y0, int x1, int y1, int color)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	if (x0 < x1)
		sx = 1;
	else
		sx = -1;
	if (y0 < y1)
		sy = 1;
	else
		sy = -1;
	err = dx - dy;
	while (1)
	{
		put_pixel(c, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void	draw_mm_ray(t_cub *c, t_mm_ray *r)
{
	int	start_x;
	int	start_y;
	int	end_x;
	int	end_y;

	get_mm_player_pos(c, &start_x, &start_y);
	end_x = start_x + (r->hit_x - c->player->x) * MM_TS;
	end_y = start_y + (r->hit_y - c->player->y) * MM_TS;
	draw_line(c, start_x, start_y, end_x, end_y, 0xFF0000);
}

void	perform_mm_ray(t_cub *c, t_mm_ray *r)
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
		r->side_dist_y = (c->player->y - r->map_y) *r->delta_dist_y;
	}
	else
	{
		r->step_y = 1;
		r->side_dist_y = (r->map_y + 1.0 - c->player->y) * r->delta_dist_y;
	}
	while (!r->hit)
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
		if (abs(r->map_x - (int)c->player->x) > 7
			|| abs(r->map_y - (int)c->player->y) > 7)
			break;
		if (c->config->map[r->map_y][r->map_x] == '1')
			r->hit = 1;
	}
	if (r->side == 0)
	{
		r->hit_x = r->map_x;
		if (r->ray_dir_x < 0)
			r->hit_x += 1.0;
		r->hit_y = c->player->y + (r->hit_x - c->player->x) * r->ray_dir_y / r->ray_dir_x;
	}
	else
	{
		r->hit_y = r->map_y;
		if (r->ray_dir_y < 0)
			r->hit_y += 1.0;
		r->hit_x = c->player->x + (r->hit_y - c->player->y) * r->ray_dir_x / r->ray_dir_y;
	}
}

void	init_mm_ray(t_cub *c, t_mm_ray *r, int i)
{
	double	camera_x;

	camera_x = 2.0 * i / MM_RAYS - 1.0;
	r->ray_dir_x = c->player->dir_x + c->player->plane_x * camera_x;
	r->ray_dir_y = c->player->dir_y + c->player->plane_y * camera_x;
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

void	draw_minimap_rays(t_cub *c)
{
	int			i;
	t_mm_ray	r;

	i = 0;
	while (i < MM_RAYS)
	{
		init_mm_ray(c, &r, i);
		perform_mm_ray(c, &r);
		draw_mm_ray(c, &r);
		i++;
	}
}
