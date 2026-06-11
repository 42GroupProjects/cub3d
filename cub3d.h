/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 20:20:32 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/09 18:24:35 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CUB3D_H
# define CUB3D_H

# include <mlx.h>

# define HEIGHT 800
# define WIDTH 800
# define TS 32

typedef struct s_images
{
	void	*i;
	int		height;
	int		width;
}	t_images;

typedef struct	s_cub
{
	void		*mlx;
	void		*win;
	int			map_h;
	int			map_w;
	char		**map;
	t_images	wall;
	t_images	floor;
}	t_cub;

#endif
