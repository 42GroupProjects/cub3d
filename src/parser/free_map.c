/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:03 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:02:42 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/** free(*s) then set *s = NULL (single string). Safe on NULL. */
void free_str(char **s)
{
	if (s && *s)
	{
		free(*s);
		*s = NULL;
	}
}

/**
 * Free a NULL-terminated string array (map rows, split words, file lines)
 * and set *arr = NULL so the freed pointer cannot be reused by mistake.
 */
void free_strarr(char ***arr)
{
	char **a;
	int i;

	if (!arr || !*arr)
		return;
	a = *arr;
	i = 0;
	while (a[i])
	{
		free(a[i]);
		i++;
	}
	free(a);
	*arr = NULL;
}

/** Free every allocation owned by `game` and NULL each field. Safe on NULL. */
void free_config(t_game *game)
{
	if (!game)
		return;
	free_str(&game->no_tex);
	free_str(&game->so_tex);
	free_str(&game->we_tex);
	free_str(&game->ea_tex);
	free_strarr(&game->map);
}
