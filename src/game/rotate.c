/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 20:23:22 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/24 21:30:00 by thanh-ng         ###   ########.fr       */
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
	c->player->dir_x = c->player->dir_x * cos_angle
		- c->player->dir_y * sin_angle;
	c->player->dir_y = old_dir_x * sin_angle
		+ c->player->dir_y * cos_angle;
	old_plane_x = c->player->plane_x;
	c->player->plane_x = c->player->plane_x * cos_angle
		- c->player->plane_y * sin_angle;
	c->player->plane_y = old_plane_x * sin_angle
		+ c->player->plane_y * cos_angle;
}
