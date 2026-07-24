/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 20:25:49 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/24 21:30:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_walkable(t_cub *c, double y, double x)
{
	int	map_x;
	int	map_y;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_y < 0
		|| map_y >= c->config->height
		|| map_x >= c->config->width)
		return (0);
	if (c->config->map[map_y][map_x] == '1'
		|| c->config->map[map_y][map_x] == ' ')
		return (0);
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

void	apply_player_input(t_cub *c)
{
	if (c->key_w)
		move(c, c->player->dir_x * MOVE_SPEED, c->player->dir_y * MOVE_SPEED);
	if (c->key_s)
		move(c, -c->player->dir_x * MOVE_SPEED, -c->player->dir_y * MOVE_SPEED);
	if (c->key_a)
		move(c, c->player->dir_y * MOVE_SPEED, -c->player->dir_x * MOVE_SPEED);
	if (c->key_d)
		move(c, -c->player->dir_y * MOVE_SPEED, c->player->dir_x * MOVE_SPEED);
	if (c->key_left)
		rotate_player(c, -ROT_SPEED);
	if (c->key_right)
		rotate_player(c, ROT_SPEED);
}

int	handle_keypress(int keycode, t_cub *c)
{
	if (keycode == KEY_ESC)
		on_close(c);
	else if (keycode == KEY_W)
		c->key_w = 1;
	else if (keycode == KEY_S)
		c->key_s = 1;
	else if (keycode == KEY_A)
		c->key_a = 1;
	else if (keycode == KEY_D)
		c->key_d = 1;
	else if (keycode == ARROW_LEFT)
		c->key_left = 1;
	else if (keycode == ARROW_RIGHT)
		c->key_right = 1;
	else if (keycode == KEY_M || keycode == KEY_M_UP)
		c->show_full_map = !c->show_full_map;
	return (0);
}

int	handle_keyrelease(int keycode, t_cub *c)
{
	if (keycode == KEY_W)
		c->key_w = 0;
	else if (keycode == KEY_S)
		c->key_s = 0;
	else if (keycode == KEY_A)
		c->key_a = 0;
	else if (keycode == KEY_D)
		c->key_d = 0;
	else if (keycode == ARROW_LEFT)
		c->key_left = 0;
	else if (keycode == ARROW_RIGHT)
		c->key_right = 0;
	return (0);
}
