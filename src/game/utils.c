/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 18:14:31 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/25 14:39:04 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* OUTDATED funtion
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
*/

/* Select the correct wall texture based on the DDA result.
* - side == 0: vertical wall (east or west)
* - side == 1: horizontal wall (north or south)
* step_x and step_y indicate which side of the wall was hit.
*
*           DDA hits a wall
*                |
*         Was the last step.
*        /                    \
*	side == 0            side == 1
*(vertical wall)      (horizontal wall)
*        |                    |
*   check step_x         check step_y
*        |                    |
* +--------------+     +--------------+
* | step_x > 0 ? |     | step_y > 0 ? |
* +--------------+     +--------------+
*      |   |                |    |
*     yes no              yes   no
*      |   |                |    |
*   EAST  WEST            SOUTH	NORTH
*/
t_texture	*get_wall_texture(t_cub *c, t_ray *r)
{
	if (r->side == 0)
	{
		if (r->step_x > 0)
			return (&c->east);
		return (&c->west);
	}
	if (r->step_y > 0)
		return (&c->south);
	return (&c->north);
}
