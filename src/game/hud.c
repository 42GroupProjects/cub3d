/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:55:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:55:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	put_fps(t_cub *c)
{
	char	s[16];
	char	*n;
	int		fps;

	if (c->dt <= 0.0)
		fps = 0;
	else
		fps = (int)(1.0 / c->dt + 0.5);
	ft_strlcpy(s, "FPS ", sizeof(s));
	n = ft_itoa(fps);
	if (!n)
		return ;
	ft_strlcat(s, n, sizeof(s));
	free(n);
	mlx_string_put(c->mlx, c->win, 12, HEIGHT - 16, 0xFFFFFF, s);
}

void	draw_hud(t_cub *c)
{
	put_fps(c);
	mlx_string_put(c->mlx, c->win, 12, HEIGHT - 32, 0xCCCCCC,
		"L mouse  SPACE/click fire");
}
