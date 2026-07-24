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

int	init_textures(t_cub *c)
{
	if (init_texture(c, c->config->no_tex, &c->north))
		return (FAILURE);
	if (init_texture(c, c->config->so_tex, &c->south))
		return (FAILURE);
	if (init_texture(c, c->config->we_tex, &c->west))
		return (FAILURE);
	if (init_texture(c, c->config->ea_tex, &c->east))
		return (FAILURE);
	return (SUCCESS);
}
