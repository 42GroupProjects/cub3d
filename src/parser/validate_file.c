/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:32 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:14:04 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
	/* reject bare ".cub" / "path/.cub" (no filename stem) */
	if (file[len - FILE_EXT_LENGTH - 1] == '/'
		|| file[len - FILE_EXT_LENGTH - 1] == '.')
		return (FALSE);
	return (TRUE);
}

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
