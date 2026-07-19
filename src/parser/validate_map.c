/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:22 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:47:22 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int is_valid_cell(char c)
{
    if (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == ' ')
        return (TRUE);
    return (FALSE);
}

int validate_characters(char **map)
{
    int i;
    int j;

    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (!is_valid_cell(map[i][j]))
                return (FALSE);
            j++;
        }
        i++;
    }
    return (TRUE);
}

static int is_spawn_cell(char c)
{
    if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
        return (TRUE);
    return (FALSE);
}

int validate_player_count(char **map)
{
    int i;
    int j;
    int count;

    count = 0;
    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (is_spawn_cell(map[i][j]))
                count++;
            j++;
        }
        i++;
    }
    return (count == 1);
}

int validate_map_borders(t_game *game)
{
    int x;
    int y;

    if (!game->map || game->height <= 0 || game->width <= 0)
        return (FALSE);
    x = 0;
    while (x < game->width)
    {
        if (game->map[0][x] != '1' || game->map[game->height - 1][x] != '1')
            return (FALSE);
        x++;
    }
    y = 0;
    while (y < game->height)
    {
        if (game->map[y][0] != '1' || game->map[y][game->width - 1] != '1')
            return (FALSE);
        y++;
    }
    return (TRUE);
}
