/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 20:25:49 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/24 21:25:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	apply_player_input(t_cub *c)
{
	double	step;
	double	turn;

	step = MOVE_SPEED * c->dt;
	turn = ROT_SPEED * c->dt;
	if (c->key_w)
		move_player(c, c->player->dir_x * step, c->player->dir_y * step);
	if (c->key_s)
		move_player(c, -c->player->dir_x * step, -c->player->dir_y * step);
	if (c->key_a)
		move_player(c, c->player->dir_y * step, -c->player->dir_x * step);
	if (c->key_d)
		move_player(c, -c->player->dir_y * step, c->player->dir_x * step);
	if (c->key_left)
		rotate_player(c, -turn);
	if (c->key_right)
		rotate_player(c, turn);
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
	else if (keycode == KEY_L)
		toggle_mouse_look(c);
	else if (keycode == KEY_SPACE)
		fire_weapon(c);
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
