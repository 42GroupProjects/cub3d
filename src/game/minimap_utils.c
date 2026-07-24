/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 20:30:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/21 20:45:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	mm_tile_color(char cell)
{
	if (cell == '1')
		return (MM_WALL);
	if (cell == '0' || cell == 'N' || cell == 'S'
		|| cell == 'E' || cell == 'W')
		return (MM_FLOOR);
	return (MM_VOID);
}
