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
