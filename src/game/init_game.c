/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 16:35:44 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/24 19:50:00 by thanh-ng         ###   ########.fr       */
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
	c->ray = ray;
	return (SUCCESS);
}

int	init_game(t_cub *c, t_game *g)
{
	c->config = g;
	c->show_full_map = 0;
	c->mouse_look = 1;
	c->mlx = mlx_init();
	if (!c->mlx)
		return (parse_error(ERR_MLX_INIT));
	c->win = mlx_new_window(c->mlx, WIDTH, HEIGHT, "cub3D");
	if (!c->win)
		return (free_cub_struct(c), parse_error(ERR_MLX_WINDOW));
	mlx_mouse_hide(c->mlx, c->win);
	c->img = mlx_new_image(c->mlx, WIDTH, HEIGHT);
	if (!c->img)
		return (free_cub_struct(c), parse_error(ERR_MLX_IMAGE));
	c->addr = mlx_get_data_addr(c->img, &c->bpp, &c->line_len, &c->endian);
	if (!c->addr)
		return (free_cub_struct(c), parse_error(ERR_MLX_DATA));
	if (init_player(c) != SUCCESS)
		return (free_cub_struct(c), parse_error(ERR_MAP_NO_PLAYER));
	if (init_ray(c) != SUCCESS)
		return (free_cub_struct(c), oom_error());
	if (init_textures(c) != SUCCESS)
		return (free_cub_struct(c), parse_error(ERR_MLX_IMAGE));
	return (SUCCESS);
}
