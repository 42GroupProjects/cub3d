#ifndef PROTOTYPES_H
# define PROTOTYPES_H

/* --- parser.c (orchestrator) --- */
/** Run the full parse pipeline into `game`. SUCCESS / FAILURE / OOM. */
int		parse_config(t_game *game, char *file);
/** Append a dup of `line` to a NULL-terminated array. NULL on alloc fail. */
char	**append_line(char **map, char *line);

/* --- validate_file.c --- */
/** Check extension + openable + not a directory. SUCCESS / FAILURE. */
int		validate_file(char *file);

/* --- parse_header.c --- */
/** Consume header lines until the map; *map_start gets the map index.
 *  SUCCESS / FAILURE (missing/dup) / OOM. */
int		parse_header(t_game *game, char **lines, int *map_start);

/* --- parse_textures.c --- */
/** Store one texture path into *dst (rejects duplicates).
 *  SUCCESS / FAILURE (dup/empty) / OOM. */
int		set_texture(char **dst, char *value);

/* --- parse_colors.c --- */
/** Parse "R,G,B" into rgb; *flag guards duplicates.
 *  SUCCESS / FAILURE (dup/invalid) / OOM. */
int		set_color(int *rgb, char *value, int *flag);

/* --- map_extract.c --- */
/** Copy the map block into game->map, set height. SUCCESS / FAILURE / OOM. */
int		extract_map(t_game *game, char **lines, int map_start);
/** Pad every row to max width with spaces. SUCCESS / OOM. */
int		normalize_map(t_game *game);

/* --- validate_map.c --- */
/** STUB: map layout is treated as valid (your flood-fill hook). SUCCESS. */
int		validate_map_layout(t_game *game);
/** TRUE if every cell is one of 0 1 N S E W space. */
int		validate_characters(char **map);
/** TRUE if there is exactly one spawn (N/S/E/W). */
int		validate_player_count(char **map);

/* --- print_map.c --- */
/** Print every row followed by a newline. */
void	print_map(char **map);
/** Debug dump of the parsed config (textures, colors, grid). */
void	print_config(t_game *game);

/* --- free_map.c --- */
/** free(*s) then *s = NULL. Safe on NULL. */
void	free_str(char **s);
/** Free a NULL-terminated string array and set *arr = NULL. Safe on NULL. */
void	free_strarr(char ***arr);
/** Free everything owned by `game` and NULL the fields. Safe on NULL. */
void	free_config(t_game *game);

/* --- utils.c --- */
/** Print "Error\n<msg>" to stderr; returns FAILURE for return-chaining. */
int		parse_error(char *msg);
/** Print the OOM message to stderr; returns OOM for return-chaining. */
int		oom_error(void);
/** Advance past leading spaces/tabs. */
char	*skip_spaces(char *s);
/** TRUE if `line` starts with id followed by a space/tab. */
int		match_id(char *line, char *id);
/** TRUE if `line` is empty or only whitespace. */
int		is_blank(char *line);

#endif
