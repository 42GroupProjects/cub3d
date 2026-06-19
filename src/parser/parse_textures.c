#include "cub3d.h"

/* Store one texture path. Rejects a second occurrence (*dst already set)
 * so a duplicate NO/SO/WE/EA does not silently leak the first path.       */
int	set_texture(char **dst, char *value)
{
	char	*path;

	value = skip_spaces(value);
	if (*dst)
		return (parse_error(ERR_DUP_TEX));
	if (*value == '\0')
		return (parse_error(ERR_EMPTY_TEX));
	path = ft_strdup(value);
	if (!path)
		return (parse_error(ERR_MALLOC));
	*dst = path;
	return (1);
}
