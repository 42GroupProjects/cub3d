/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 22:05:34 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/08/22 23:34:59 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_walkable(t_cub *c, double y, double x)
{
	double	dx;
	double	dy;
	int		mx;
	int		my;

	dy = -COLLISION_PAD;
	while (dy <= COLLISION_PAD)
	{
		dx = -COLLISION_PAD;
		while (dx <= COLLISION_PAD)
		{
			mx = (int)(x + dx);
			my = (int)(y + dy);
			if (mx < 0 || my < 0 || my >= c->config->height
				|| mx >= c->config->width
				|| c->config->map[my][mx] == '1'
				|| c->config->map[my][mx] == ' ')
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

static double	frame_dt(void)
{
	static double	last;
	struct timeval	tv;
	double			now;
	double			dt;

	gettimeofday(&tv, NULL);
	now = (double)tv.tv_sec + (double)tv.tv_usec * 0.000001;
	dt = now - last;
	last = now;
	if (dt <= 0.0 || dt > 0.05)
		dt = 0.016;
	return (dt);
}

void	apply_player_input(t_cub *c)
{
	double	dt;
	double	step;
	double	turn;

	dt = frame_dt();
	step = MOVE_SPEED * (dt * 20.0);
	turn = ROT_SPEED * (dt * 20.0);
	if (c->key_w)
		move(c, c->player->dir_x * step, c->player->dir_y * step);
	if (c->key_s)
		move(c, -c->player->dir_x * step, -c->player->dir_y * step);
	if (c->key_a)
		move(c, c->player->dir_y * step, -c->player->dir_x * step);
	if (c->key_d)
		move(c, -c->player->dir_y * step, c->player->dir_x * step);
	if (c->key_left)
		rotate_player(c, -turn);
	if (c->key_right)
		rotate_player(c, turn);
}
