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

#endif
