/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 20:25:49 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/14 18:00:47 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cub3d.h"
/*
static int	is_walkable(t_cub *c, int y, int x)
{
	double	i;
	double	j;

	i = c->player->x;
	j = c->player->y;
	if (c->config->map[(int)i][(int)j] == '1')
		return (0);
	
}
*/
int	handle_keypress(int keycode, t_cub *c)
{
	(void)c;
	if (keycode == 65307)
	{
		printf("ESC: %d pressed\n", keycode);
		on_close(c);
	}
	else if (keycode == 65362 || keycode == 119)
		printf("UP: %d pressed\n", keycode);
	else if (keycode == 65364 || keycode == 115)
		printf("DOWN: %d pressed\n", keycode);
	else if (keycode == 65361 || keycode == 97)
		printf("LEFT: %d pressed\n", keycode);
	else if (keycode == 65363 || keycode == 100)
		printf("RIGHT: %d pressed\n", keycode);
	else
		printf("UNDEFINED INPUT: %d pressed\n", keycode);
	return (0);
}
