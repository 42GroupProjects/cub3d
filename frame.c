/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 20:57:40 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/09 18:26:33 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	render_map(t_cub *cub, int i, int j)
{
	if (cub->map[i][j] == '1')
		mlx_put_image_to_window(cub->mlx, cub->win, //missing);
	else if (cub->map[i][j] == '0')
		mlx_put_image_to_window(cub->mlx, cub->win, //);
}

int	render_next_frame(t_cub *cub)
{
	int	i;
	int	j;

	i = 0;
	while (i < cub.map_h)
	{
		j = 0;
		while (j < cub.map_w)
		{
			render_map(cub, i, j);
			j++;
		}
		i++;
	}
	return (0);
}
