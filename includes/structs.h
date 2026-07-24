/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 20:55:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 18:44:37 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

/*
** Parsed scene configuration: header (textures + colors) and the map grid.
**   no/so/we/ea_tex — texture paths on the heap; NULL until set.
**   floor / ceiling — RGB triplets; meaningful only when has_* is TRUE.
**   has_floor/ceiling — duplicate + presence guards for the F / C lines.
**   map             — NULL-terminated rows, each padded to `width` spaces.
**   width / height  — grid dimensions after normalize_map().
*/
typedef struct s_game
{
	char	*no_tex;
	char	*so_tex;
	char	*we_tex;
	char	*ea_tex;
	int		floor[3];
	int		ceiling[3];
	int		has_floor;
	int		has_ceiling;
	char	**map;
	int		width;
	int		height;
}	t_game;

/*
** Player configuration:
**	float for x and y position in map
**	float for player direction
*/
typedef struct s_player
{
	char	c;
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

/*
** Raycast struct:
*/
typedef struct s_ray
{
	double	camera_x;
	double	camera_y;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	double	perp_wall_dist;
	int		line_height;
	int		draw_start;
	int		draw_end;
	double	wall_x;
	int		tx_x;
}	t_ray;

/*
** Texture struct
*/
typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}	t_texture;

/*
** Main struct for raycaster and game.
*/
typedef struct s_cub
{
	void				*mlx;
	void				*win;
	void				*img;
	char				*addr;
	int					bpp;
	int					line_len;
	int					endian;
	struct s_game		*config;
	struct s_player		*player;
	struct s_ray		*ray;
	struct s_texture	north;
	struct s_texture	east;
	struct s_texture	south;
	struct s_texture	west;
}	t_cub;
#endif
