/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:25:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:25:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	cell_blocked(t_cub *c, int map_x, int map_y)
{
	if (map_x < 0 || map_y < 0
		|| map_y >= c->config->height
		|| map_x >= c->config->width)
		return (1);
	if (c->config->map[map_y][map_x] == '1'
		|| c->config->map[map_y][map_x] == ' ')
		return (1);
	return (0);
}

static int	is_walkable(t_cub *c, double y, double x)
{
	double	dx;
	double	dy;

	dy = -COLLISION_PAD;
	while (dy <= COLLISION_PAD)
	{
		dx = -COLLISION_PAD;
		while (dx <= COLLISION_PAD)
		{
			if (cell_blocked(c, (int)(x + dx), (int)(y + dy)))
				return (0);
			dx += 2.0 * COLLISION_PAD;
		}
		dy += 2.0 * COLLISION_PAD;
	}
	return (1);
}

void	move_player(t_cub *c, double dx, double dy)
{
	double	new_x;
	double	new_y;

	new_x = c->player->x + dx;
	new_y = c->player->y + dy;
	if (is_walkable(c, c->player->y, new_x))
		c->player->x = new_x;
	if (is_walkable(c, new_y, c->player->x))
		c->player->y = new_y;
}
