/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crosshair.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:35:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:35:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_crosshair(t_cub *cub)
{
	int	cx;
	int	cy;
	int	i;

	cx = WIDTH / 2;
	cy = HEIGHT / 2;
	i = -CROSS_SIZE;
	while (i <= CROSS_SIZE)
	{
		if (i < -CROSS_GAP || i > CROSS_GAP)
		{
			put_pixel(cub, cx + i, cy, CROSS_COLOR);
			put_pixel(cub, cx, cy + i, CROSS_COLOR);
		}
		i++;
	}
}
