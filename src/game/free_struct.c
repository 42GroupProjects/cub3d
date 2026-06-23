/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:53:58 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/23 15:21:43 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	free_player_struct(t_player *p)
{
	p->c = 0;
	p->x = 0;
	p->y = 0;
	p->dir_x = 0;
	p->dir_y = 0;
	p->plane_x = 0;
	p->plane_y = 0;
	free(p);
}

static void	free_ray_struct(t_ray *r)
{
	r->camera_x = 0;
	r->ray_dir_x = 0;
	r->ray_dir_y = 0;
	r->map_x = 0;
	r->map_y = 0;
	r->side_dist_x = 0;
	r->side_dist_y = 0;
	r->delta_dist_x = 0;
	r->delta_dist_y = 0;
	r->step_x = 0;
	r->step_y = 0;
	r->hit = 0;
	r->side = 0;
	r->perp_wall_dist = 0;
	free(r);
}

void	free_cub_struct(t_cub *cub)
{
	if (cub->img)
	{
		mlx_destroy_image(cub->mlx, cub->img);
		free(cub->img);
		cub->img = NULL;
	}
	if (cub->win)
		mlx_destroy_window(cub->mlx, cub->win);
	if (cub->mlx)
	{
		mlx_destroy_display(cub->mlx);
		cub->mlx = NULL;
	}
	if (cub->addr)
	cub->bpp = 0;
	cub->line_len = 0;
	cub->endian = 0;
	if (cub->config)
		cub->config = NULL;	//gets already freed
	if (cub->player)
	{
		free_player_struct(cub->player);
		cub->player = NULL;
	}
	if (cub->ray)
	{
		free_ray_struct(cub->ray);
		cub->ray = NULL;
	}
	cub = NULL;
}

