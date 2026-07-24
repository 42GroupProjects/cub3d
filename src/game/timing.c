/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:50:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:50:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static double	now_seconds(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((double)tv.tv_sec + (double)tv.tv_usec * 0.000001);
}

void	update_frame_time(t_cub *c)
{
	double	now;

	now = now_seconds();
	if (c->last_frame <= 0.0)
		c->dt = DT_DEFAULT;
	else
		c->dt = now - c->last_frame;
	if (c->dt > DT_MAX)
		c->dt = DT_MAX;
	if (c->dt < 0.0)
		c->dt = DT_DEFAULT;
	c->last_frame = now;
}

void	update_view_bob(t_cub *c)
{
	double	decay;

	if (c->key_w || c->key_a || c->key_s || c->key_d)
		c->head_bob += HEAD_BOB_SPEED * c->dt;
	else
	{
		decay = WEAPON_BOB_DECAY * c->dt;
		if (decay > 1.0)
			decay = 1.0;
		c->head_bob *= (1.0 - decay);
	}
	c->view_bob_y = (int)(sin(c->head_bob) * HEAD_BOB_AMT);
}

void	apply_ray_bob(t_cub *c, t_ray *r)
{
	r->draw_start += c->view_bob_y;
	r->draw_end += c->view_bob_y;
	if (r->draw_start < 0)
		r->draw_start = 0;
	if (r->draw_end >= HEIGHT)
		r->draw_end = HEIGHT - 1;
}
