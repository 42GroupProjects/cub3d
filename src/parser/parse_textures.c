/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:22 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 18:45:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	rstrip_spaces(char *s)
{
	int	len;

	len = ft_strlen(s);
	while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t'))
	{
		s[len - 1] = '\0';
		len--;
	}
}

static int	is_hidden_basename(char *path)
{
	char	*base;

	base = path;
	while (*path)
	{
		if (*path == '/')
			base = path + 1;
		path++;
	}
	return (base[0] == '.');
}

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
		return (oom_error());
	rstrip_spaces(path);
	if (path[0] == '\0' || is_hidden_basename(path))
	{
		free(path);
		return (parse_error(ERR_EMPTY_TEX));
	}
	*dst = path;
	return (SUCCESS);
}
