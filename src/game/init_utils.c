/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 14:37:26 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/23 14:58:48 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	get_player_fov(t_player *player)
{
	if (player->c == 'N')
	{
		player->plane_y = 0;
		player->plane_x = 0.66;
	}
	else if (player->c == 'E')
	{
		player->plane_y = 0.66;
		player->plane_x = 0;
	}
	else if (player->c == 'S')
	{
		player->plane_y = 0;
		player->plane_x = -0.66;
	}
	else
	{
		player->plane_y = -0.66;
		player->plane_x = 0;
	}
}

static void	get_player_direction(t_player *player)
{
	if (player->c == 'N')
	{
		player->dir_y = -1;
		player->dir_x = 0;
	}
	else if (player->c == 'E')
	{
		player->dir_y = 0;
		player->dir_x = 1;
	}
	else if (player->c == 'S')
	{
		player->dir_y = 1;
		player->dir_x = 0;
	}
	else
	{
		player->dir_y = 0;
		player->dir_x = -1;
	}
}

static int	find_player_pos(t_game *g, t_player *player)
{
	int	y;
	int	x;

	y = 0;
	while (g->map[y])
	{
		x = 0;
		while (g->map[y][x])
		{
			if (g->map[y][x] == 'N' || g->map[y][x] == 'E'
				|| g->map[y][x] == 'S' || g->map[y][x] == 'W')
			{
				player->c = g->map[y][x];
				player->y = y + 0.5;
				player->x = x + 0.5;
				g->map[y][x] = '0';
				return (SUCCESS);
			}
			x++;
		}
		y++;
	}
	return (FAILURE);
}

/**
 * Creating and inializeing player struct.
 * find player position on map and change the tile after finding player to 0
 * find the direction the player is facing
 * get the field of view from the player depending on the direction.
 * assigning the player struct to the pointer in the overall struct.
 */
int	init_player(t_cub *c)
{
	t_player	*player;

	player = malloc(sizeof(t_player) * 1);
	if (!player)
		return (FAILURE);
	if (find_player_pos(c->config, player) != SUCCESS)
		return (FAILURE);
	get_player_direction(player);
	get_player_fov(player);
	print_player_struct(player);
	c->player = player;
	return (SUCCESS);
}
