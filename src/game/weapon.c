/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:45:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:55:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	weapon_pixel(t_texture *t, int x, int y)
{
	char	*pixel;

	pixel = t->addr + y * t->line_len + x * (t->bpp / 8);
	return (*(unsigned int *)pixel);
}

static void	update_weapon_motion(t_cub *c)
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
	if (c->weapon_kick > 0.0)
		c->weapon_kick -= c->dt;
	if (c->weapon_kick < 0.0)
		c->weapon_kick = 0.0;
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

void	fire_weapon(t_cub *c)
{
	if (c->weapon_kick <= 0.0)
		c->weapon_kick = FIRE_KICK_TIME;
}

void	draw_weapon(t_cub *c)
{
	int	ox;
	int	oy;
	int	kick;

	if (!c->weapon.img)
		return ;
	update_weapon_motion(c);
	kick = (int)((c->weapon_kick / FIRE_KICK_TIME) * FIRE_KICK_PX);
	ox = (WIDTH - c->weapon.width * WEAPON_SCALE) / 2
		+ (int)(sin(c->weapon_bob) * WEAPON_BOB_X);
	oy = HEIGHT - c->weapon.height * WEAPON_SCALE + 8
		+ (int)(fabs(cos(c->weapon_bob * 2.0)) * WEAPON_BOB_Y) + kick;
	blit_weapon(c, ox, oy);
}
