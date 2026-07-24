/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:27:06 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/24 21:35:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	print_player_struct(t_player *player)
{
	printf("----------------------------------------------\n");
	printf("Player position: [%f][%f]\n", player->y, player->x);
	printf("Player direction: [%f][%f]\n", player->dir_y, player->dir_x);
	printf("Camera plane: [%f][%f]\n", player->plane_y, player->plane_x);
	printf("%s\n", __func__);
	printf("----------------------------------------------\n");
}

void	print_ray_struct(t_ray *ray)
{
	printf("----------------------------------------------\n");
	printf("Camera_x: %f\n", ray->camera_x);
	printf("RAY DIR X: %f | Y: %f\n", ray->ray_dir_x, ray->ray_dir_y);
	printf("map x: %d | y: %d\n", ray->map_x, ray->map_y);
	printf("Side dist X: %f | Y: %f\n", ray->side_dist_x, ray->side_dist_y);
	printf("Delta dist X: %f | Y: %f\n", ray->delta_dist_x, ray->delta_dist_y);
	printf("step x: %d | y: %d\n", ray->step_x, ray->step_y);
	printf("hit: %d | side: %d\n", ray->hit, ray->side);
	printf("perp wall dist: %f\n", ray->perp_wall_dist);
	printf("%s\n", __func__);
	printf("----------------------------------------------\n");
}
