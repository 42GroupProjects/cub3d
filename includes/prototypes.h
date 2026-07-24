/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototypes.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 20:55:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 18:26:44 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPES_H
# define PROTOTYPES_H

int			parse_config(t_game *game, char *file);
char		**append_line(char **map, char *line);
int			validate_file(char *file);
int			parse_header(t_game *game, char **lines, int *map_start);
int			set_texture(char **dst, char *value);
int			set_color(int *rgb, char *value, int *flag);
int			extract_map(t_game *game, char **lines, int map_start);
int			normalize_map(t_game *game);
int			validate_map_layout(t_game *game);
int			check_map_closed(t_game *game);
void		print_map(char **map);
void		print_config(t_game *game);
void		free_str(char **s);
void		free_strarr(char ***arr);
void		free_config(t_game *game);
void		clean_exit(t_cub *c, int code);
int			parse_error(char *msg);
int			oom_error(void);
char		*skip_spaces(char *s);
int			match_id(char *line, char *id);
int			is_blank(char *line);
int			init_game(t_cub *c, t_game *g);
int			init_player(t_cub *c);
int			init_textures(t_cub *c);
void		free_textures(t_cub *c);
void		print_player_struct(t_player *player);
void		print_ray_struct(t_ray *ray);
void		free_cub_struct(t_cub *cub);
int			render(t_cub *cub);
void		cast_single_ray(t_cub *cub);
void		put_pixel(t_cub *cub, int x, int y, int color);
void		cast_ray(t_cub *c, int x);
int			get_wall_color(t_ray *r);
int			handle_keypress(int keycode, t_cub *c);
void		on_close(t_cub *c);
int			on_x(t_cub *c);
void		rotate_player(t_cub *c, double angle);
void		calculate_wall_x(t_cub *c, t_ray *r);
void		calculate_tx_x(t_texture *t, t_ray *r);
void		draw_textured_line(t_cub *c, t_texture *t, t_ray *r, int x);
t_texture	*get_wall_texture(t_cub *c, t_ray *r);

int			mm_tile_color(char cell);
void		mm_put_pixel_blend(t_cub *cub, int x, int y, int color);
void		mm_draw_dot(t_cub *cub, int cx, int cy, int size);
void		mm_draw_facing(t_cub *cub, int cx, int cy, int len);
int			mm_full_tile_size(t_cub *cub);
void		draw_radar_minimap(t_cub *cub);
void		draw_full_map_overlay(t_cub *cub);

#endif
