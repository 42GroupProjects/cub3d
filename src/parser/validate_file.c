#include "cub3d.h"

/** TRUE if `file` ends in ".cub" with at least one char before it. */
static int	check_extension(char *file)
{
	int	len;

	if (!file)
		return (FALSE);
	len = ft_strlen(file);
	if (len <= FILE_EXT_LENGTH)
		return (FALSE);
	if (ft_strncmp(file + len - FILE_EXT_LENGTH, ".cub", FILE_EXT_LENGTH) != 0)
		return (FALSE);
	return (TRUE);
}

/** TRUE if `file` is a regular readable file (a directory read()s as -1). */
static int	check_not_directory(char *file)
{
	int		fd;
	char	buffer;
	int		ret;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (FALSE);
	ret = read(fd, &buffer, 1);
	close(fd);
	if (ret < 0)
		return (FALSE);
	return (TRUE);
}

/** Extension + openable + not a directory. SUCCESS / FAILURE. */
int	validate_file(char *file)
{
	int	fd;

	if (!check_extension(file))
		return (parse_error(ERR_INVALID_EXT));
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (parse_error(ERR_FILE_OPEN));
	close(fd);
	if (!check_not_directory(file))
		return (parse_error(ERR_FILE_READ));
	return (SUCCESS);
}
