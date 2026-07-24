/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:45:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:45:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	weapon_pixel(t_texture *t, int x, int y)
{
	char	*pixel;

	pixel = t->addr + y * t->line_len + x * (t->bpp / 8);
	return (*(unsigned int *)pixel);
}

static void	update_weapon_bob(t_cub *c)
{
	double	decay;

	if (c->key_w || c->key_a || c->key_s || c->key_d)
		c->weapon_bob += WEAPON_BOB_SPEED * c->dt;
	else
	{
		decay = WEAPON_BOB_DECAY * c->dt;
		if (decay > 1.0)
			decay = 1.0;
		c->weapon_bob *= (1.0 - decay);
	}
}

static void	blit_weapon(t_cub *c, int ox, int oy)
{
	int	x;
	int	y;
	int	color;
	int	tx;
	int	ty;

	y = 0;
	while (y < c->weapon.height * WEAPON_SCALE)
	{
		x = 0;
		while (x < c->weapon.width * WEAPON_SCALE)
		{
			tx = x / WEAPON_SCALE;
			ty = y / WEAPON_SCALE;
			color = weapon_pixel(&c->weapon, tx, ty);
			if ((color & 0x00FFFFFF) != (WEAPON_KEY & 0x00FFFFFF))
				put_pixel(c, ox + x, oy + y, color);
			x++;
		}
		y++;
	}
}

void	draw_weapon(t_cub *c)
{
	int	ox;
	int	oy;
	int	w;
	int	h;

	if (!c->weapon.img)
		return ;
	update_weapon_bob(c);
	w = c->weapon.width * WEAPON_SCALE;
	h = c->weapon.height * WEAPON_SCALE;
	ox = (WIDTH - w) / 2 + (int)(sin(c->weapon_bob) * WEAPON_BOB_X);
	oy = HEIGHT - h + 8
		+ (int)(fabs(cos(c->weapon_bob * 2.0)) * WEAPON_BOB_Y);
	blit_weapon(c, ox, oy);
}
