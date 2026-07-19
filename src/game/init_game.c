/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 16:35:44 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/23 15:03:45 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	init_ray(t_cub *c)
{
	t_ray	*ray;

	ray = malloc(sizeof(t_ray) * 1);
	if (!ray)
		return (FAILURE);
	ft_memset(ray, 0, sizeof(t_ray));
	print_ray_struct(ray);
	// FIXME: heap ray is unused — cast_ray uses a stack t_ray; free this or use it
	c->ray = ray;
	return (SUCCESS);
}

int	init_game(t_cub *c, t_game *g)
{
	c->config = g;
	c->mlx = mlx_init();
	if (!c->mlx)
		return (parse_error(ERR_MLX_INIT));
	c->win = mlx_new_window(c->mlx, WIDTH, HEIGHT, "cub3d");
	if (!c->win)
		return (parse_error(ERR_MLX_WINDOW));
	c->img = mlx_new_image(c->mlx, WIDTH, HEIGHT);
	if (!c->img)
		return (parse_error(ERR_MLX_IMAGE));
	c->addr = mlx_get_data_addr(c->img, &c->bpp, &c->line_len, &c->endian);
	if (!c->addr)
		return (parse_error(ERR_MLX_DATA));
	// FIXME: load NO/SO/WE/EA via mlx_xpm_file_to_image here (paths in c->config)
	if (init_player(c) != SUCCESS)
		printf("%s failed\n", __func__);
	if (init_ray(c) != SUCCESS)
		printf("%s failed\n", __func__);
	// FIXME: on any fail above, destroy mlx/win/img and return FAILURE (still returns SUCCESS)
	return (SUCCESS);
}
