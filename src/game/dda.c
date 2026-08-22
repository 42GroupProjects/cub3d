/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 21:25:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/08/22 23:34:03 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	dda_step(t_ray *r)
{
	if (r->side_dist_x < r->side_dist_y)
	{
		r->side_dist_x += r->delta_dist_x;
		r->map_x += r->step_x;
		r->side = 0;
	}
	else
	{
		r->side_dist_y += r->delta_dist_y;
		r->map_y += r->step_y;
		r->side = 1;
	}
}

static int	dda_hit(t_cub *c, t_ray *r)
{
	char	tile;

	if (r->map_x < 0 || r->map_y < 0
		|| r->map_y >= c->config->height
		|| r->map_x >= c->config->width)
		return (1);
	tile = c->config->map[r->map_y][r->map_x];
	return (tile == '1' || tile == ' ');
}

void	perform_dda(t_cub *c, t_ray *r)
{
	int	steps;
	int	limit;

	steps = 0;
	limit = (c->config->width + c->config->height) * 2;
	while (r->hit == 0)
	{
		if (++steps > limit)
		{
			r->hit = 1;
			return ;
		}
		dda_step(r);
		if (dda_hit(c, r))
			r->hit = 1;
	}
}
