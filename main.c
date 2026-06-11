/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 20:18:11 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/10 20:42:44 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	free_map(char **arr, int i)
{
	if (i <= 0)
		return ;
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
}

static char	**get_map(int h, int w)
{
	char	**map;
	int		i;
	int		j;
	map = malloc(sizeof(char *) * (h + 1));
	if (!map)
		return (NULL);
	i = -1;
	while (++i < h)
	{
		j = -1;
		map[i] = malloc(sizeof(char) * (w + 1));
		if (!map[i])
			return (free_map(map, i), NULL);	//need to free this
		while (++j < w)
		{
			if (i == 0 || i == h - 1)
				map[i][j] = '1';
			else if (j == 0 || j == w - 1)
				map[i][j] = '1';
			else
				map[i][j] = '0';
		}
		if (j == w)
			map[i][j] = '\0';
	}
	if (i == h)
		map[i] = NULL;
	return (map);
}

int	main(void)
{
	t_cub	*cub;
	int		i;
	int		j;

	cub = malloc(sizeof(t_cub) * 1);
	if (!cub)
		return (1);
	cub->mlx = NULL;
	cub->win = NULL;
	cub->map = NULL;
	cub->map_h = 8;
	cub->map_w = 8;
	cub->map = get_map(cub->map_h, cub->map_w);
	if (!cub->map)
		return (3);
	if (init_images(&cub) != 0)
		return (4);
	cub->mlx = mlx_init();
	if (!cub->mlx)
		return (1);	//free_everything
	cub->win = mlx_new_window(cub->mlx, WIDTH, HEIGHT, "TEST");
	if (!cub->win)
		return (2);	//free everything
	//mlx_hook(cub.win, 2, 1L << 0, handle_keypress, &cub);
	mlx_loop_hook(cub->mlx, render_next_frame, &cub);
	//mlx_hook(cub.win, 17, 0, on_x, &cub);
	mlx_loop(cub->mlx);
	//i = 0;
	//while (cub->map[i])
	//	printf("%s\n", cub->map[i++]);
}
