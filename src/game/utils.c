/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 18:14:31 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/05 18:17:09 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_wall_color(t_ray *r)
{
	if (r->side == 0)
	{
		if (r->step_x > 0)
			return (0xFF0000);
		return (0x00FF00);
	}
	if (r->step_y > 0)
		return (0x0000FF);
	return (0xFFFF00);
}
