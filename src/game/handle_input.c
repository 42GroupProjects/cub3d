/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 20:25:49 by lwittwer          #+#    #+#             */
/*   Updated: 2026/08/22 23:05:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	handle_keypress(int keycode, t_cub *c)
{
	if (keycode == KEY_ESC)
		on_close(c);
	else if (keycode == M_UP)
		c->key_w = 1;
	else if (keycode == M_DOWN)
		c->key_s = 1;
	else if (keycode == M_LEFT)
		c->key_a = 1;
	else if (keycode == M_RIGHT)
		c->key_d = 1;
	else if (keycode == ARROW_LEFT)
		c->key_left = 1;
	else if (keycode == ARROW_RIGHT)
		c->key_right = 1;
	else if (keycode == MM_TOGGLE)
		c->show_minimap = !c->show_minimap;
	return (0);
}

int	handle_keyrelease(int keycode, t_cub *c)
{
	if (keycode == M_UP)
		c->key_w = 0;
	else if (keycode == M_DOWN)
		c->key_s = 0;
	else if (keycode == M_LEFT)
		c->key_a = 0;
	else if (keycode == M_RIGHT)
		c->key_d = 0;
	else if (keycode == ARROW_LEFT)
		c->key_left = 0;
	else if (keycode == ARROW_RIGHT)
		c->key_right = 0;
	return (0);
}
