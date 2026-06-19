#include "cub3d.h"

/* File must end in ".cub" (and have something before the extension). */
static int	check_extension(char *file)
{
	int	len;

	if (!file)
		return (0);
	len = ft_strlen(file);
	if (len <= 4)
		return (0);
	if (ft_strncmp(file + len - 4, ".cub", 4) != 0)
		return (0);
	return (1);
}

/* Reject directories: open() succeeds on a dir but read() returns -1. */
static int	check_not_directory(char *file)
{
	int		fd;
	char	buffer;
	int		ret;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (0);
	ret = read(fd, &buffer, 1);
	close(fd);
	if (ret < 0)
		return (0);
	return (1);
}

/* Extension + openable + not-a-directory. Prints the precise error. */
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
	return (1);
}
