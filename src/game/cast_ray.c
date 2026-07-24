/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:35:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:44:46 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	cast_ray(t_cub *c, int x)
{
	t_ray		r;
	t_texture	*tx;

	setup_ray(c, &r, x);
	calculate_step(c, &r);
	perform_dda(c, &r);
	calculate_perp_wall_dist(c, &r);
	calculate_line_height(&r);
	apply_ray_bob(c, &r);
	calculate_wall_x(c, &r);
	tx = get_wall_texture(c, &r);
	calculate_tx_x(tx, &r);
	draw_textured_line(c, tx, &r, x);
}
