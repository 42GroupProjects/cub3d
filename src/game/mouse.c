/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:20:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:55:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	toggle_mouse_look(t_cub *c)
{
	c->mouse_look = !c->mouse_look;
	c->mouse_ready = 0;
	if (c->mouse_look)
		mlx_mouse_hide(c->mlx, c->win);
	else
		mlx_mouse_show(c->mlx, c->win);
}

void	apply_mouse_look(t_cub *c)
{
	int	x;
	int	y;
	int	center_x;
	int	center_y;
	int	delta_x;

	if (!c->mouse_look)
		return ;
	center_x = WIDTH / 2;
	center_y = HEIGHT / 2;
	mlx_mouse_get_pos(c->mlx, c->win, &x, &y);
	if (!c->mouse_ready)
	{
		mlx_mouse_move(c->mlx, c->win, center_x, center_y);
		c->mouse_ready = 1;
		return ;
	}
	delta_x = x - center_x;
	if (delta_x != 0)
		rotate_player(c, (double)delta_x * MOUSE_SENS);
	mlx_mouse_move(c->mlx, c->win, center_x, center_y);
}
