/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 20:25:49 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/21 17:47:25 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
static int	is_walkable(t_cub *c, int y, int x)
{
	int	map_x;
	int	map_y;

	map_x = (int)x;
	map_y = (int)y;
	if (c->config->map[map_y][map_x] == '1' || c->config->map[map_y][map_x] == ' ') // TODO: checked for ' ' 
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
		on_close(c);
	else if (keycode == 119)
		move(c, c->player->dir_x * MOVE_SPEED, c->player->dir_y * MOVE_SPEED);
	else if (keycode == 115)
		move(c, -c->player->dir_x * MOVE_SPEED, -c->player->dir_y * MOVE_SPEED);
	else if (keycode == 97)
		move(c, c->player->dir_y * MOVE_SPEED, -c->player->dir_x * MOVE_SPEED);
	else if (keycode == 100)
		move(c, -c->player->dir_y * MOVE_SPEED, c->player->dir_x * MOVE_SPEED);
	else if (keycode == 65361)	//left
		rotate_player(c, -ROT_SPEED);
	else if (keycode == 65363)	//right
		rotate_player(c, ROT_SPEED);
	else
		printf("UNDEFINED INPUT: %d pressed\n", keycode); // TODO: strip all debug printf before eval
	return (0);
}
