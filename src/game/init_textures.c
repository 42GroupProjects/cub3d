/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 14:39:58 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/24 18:47:02 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	init_texture(t_cub *c, char *path, t_texture *tx)
{
	tx->img = mlx_xpm_file_to_image(c->mlx, path, &tx->width, &tx->height);
	if (!tx->img)
		return (FAILURE);
	tx->addr = mlx_get_data_addr(tx->img, &tx->bpp, &tx->line_len, &tx->endian);
	if (!tx->addr)
		return (FAILURE);
	return (SUCCESS);
}

void	free_textures(t_cub *c)
{
	if (!c || !c->mlx)
		return ;
	if (c->north.img)
		mlx_destroy_image(c->mlx, c->north.img);
	if (c->south.img)
		mlx_destroy_image(c->mlx, c->south.img);
	if (c->west.img)
		mlx_destroy_image(c->mlx, c->west.img);
	if (c->east.img)
		mlx_destroy_image(c->mlx, c->east.img);
	if (c->weapon.img)
		mlx_destroy_image(c->mlx, c->weapon.img);
	c->north.img = NULL;
	c->south.img = NULL;
	c->west.img = NULL;
	c->east.img = NULL;
	c->weapon.img = NULL;
}

int	init_textures(t_cub *c)
{
	if (init_texture(c, c->config->no_tex, &c->north)
		|| init_texture(c, c->config->so_tex, &c->south)
		|| init_texture(c, c->config->we_tex, &c->west)
		|| init_texture(c, c->config->ea_tex, &c->east)
		|| init_texture(c, WEAPON_PATH, &c->weapon))
	{
		free_textures(c);
		return (FAILURE);
	}
	return (SUCCESS);
}
