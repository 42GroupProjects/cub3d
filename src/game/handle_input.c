/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 20:25:49 by lwittwer          #+#    #+#             */
/*   Updated: 2026/08/22 17:09:25 by lwittwer         ###   ########.fr       */
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

static void	move(t_cub *c, double dx, double dy)
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

int	handle_keypress(int keycode, t_cub *c)
{
	if (keycode == 65307)
		on_close(c);
	else if (keycode == M_UP)
		move(c, c->player->dir_x * MOVE_SPEED, c->player->dir_y * MOVE_SPEED);
	else if (keycode == M_DOWN)
		move(c, -c->player->dir_x * MOVE_SPEED, -c->player->dir_y * MOVE_SPEED);
	else if (keycode == M_LEFT)
		move(c, c->player->dir_y * MOVE_SPEED, -c->player->dir_x * MOVE_SPEED);
	else if (keycode == M_RIGHT)
		move(c, -c->player->dir_y * MOVE_SPEED, c->player->dir_x * MOVE_SPEED);
	else if (keycode == ARROW_LEFT)
		rotate_player(c, -ROT_SPEED);
	else if (keycode == ARROW_RIGHT)
		rotate_player(c, ROT_SPEED);
	else if (keycode == MM_TOGGLE)
		c->show_minimap = !c->show_minimap;
	return (0);
}
