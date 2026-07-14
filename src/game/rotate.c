/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 20:23:22 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/14 22:39:31 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	rotate_player(t_cub *c, double angle)
{
	double	old_dir_x;
	double	old_plane_x;
	double	cos_angle;
	double	sin_angle;

	cos_angle = cos(angle);
	sin_angle = sin(angle);
	old_dir_x = c->player->dir_x;
	c->player->dir_x = c->player->dir_x * cos_angle - c->player->dir_y * sin_angle;
	c->player->dir_y = old_dir_x * sin_angle + c->player->dir_y * cos_angle;
	old_plane_x = c->player->plane_x;
	c->player->plane_x = c->player->plane_x * cos_angle - c->player->plane_y * sin_angle;	
	c->player->plane_y = old_plane_x * sin_angle + c->player->plane_y * cos_angle;
	printf("dir = (%f, %f)\n", c->player->dir_x, c->player->dir_y);
	printf("plane = (%f, %f)\n", c->player->plane_x, c->player->plane_y);
	printf("length: %f\n", sqrt(c->player->dir_x * c->player->dir_x + c->player->dir_y * c->player->dir_y));
}
