/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 20:25:49 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/15 16:54:05 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
static int	is_walkable(t_cub *c, int y, int x)
{
	int	map_x;
	int	map_y;

	map_x = (int)x;
	map_y = (int)y;
	// FIXME: reject OOB / ' ' cells too — only checking '1' lets you walk off the map
	if (c->config->map[map_y][map_x] == '1')
		return (0);
	return (1);
}

static void	move(t_cub *c, double dx, double dy)
{
	double	new_x;
	double	new_y;

	new_x = c->player->x + dx;
	new_y = c->player->y + dy;
	if (is_walkable(c, new_y, new_x))
	{
		c->player->x = new_x;
		c->player->y = new_y;
	}
}

int	handle_keypress(int keycode, t_cub *c)
{

	if (keycode == 65307)
	{
		printf("ESC: %d pressed\n", keycode);
		on_close(c);
	}
	else if (keycode == 65362 || keycode == 119)
	{
		printf("UP: %d pressed\n", keycode);
		move(c, c->player->dir_x * MOVE_SPEED, c->player->dir_y * MOVE_SPEED);
	}
	else if (keycode == 65364 || keycode == 115)
	{
		printf("DOWN: %d pressed\n", keycode);
		move(c, -c->player->dir_x * MOVE_SPEED, -c->player->dir_y * MOVE_SPEED);
	}
	// FIXME: subject wants ←/→ to rotate look; arrows currently strafe — swap with Q/E or drop Q/E
	else if (keycode == 65361 || keycode == 97)
	{
		printf("LEFT: %d pressed\n", keycode);
		move(c, c->player->dir_y * MOVE_SPEED, -c->player->dir_x * MOVE_SPEED);
	}
	else if (keycode == 65363 || keycode == 100)
	{
		printf("RIGHT: %d pressed\n", keycode);
		move(c, -c->player->dir_y * MOVE_SPEED, c->player->dir_x * MOVE_SPEED);
	}
	else if (keycode == 113)	//left
		rotate_player(c, -ROT_SPEED);
	else if (keycode == 101)	//right
		rotate_player(c, ROT_SPEED);
	else
		printf("UNDEFINED INPUT: %d pressed\n", keycode); // FIXME: strip all debug printf before eval
	return (0);
}
