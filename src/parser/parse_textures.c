/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:22 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:14:04 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int set_texture(char **dst, char *value)
{
	char *path;

	value = skip_spaces(value);
	if (*dst)
		return (parse_error(ERR_DUP_TEX));
	if (*value == '\0')
		return (parse_error(ERR_EMPTY_TEX));
	path = ft_strdup(value);
	if (!path)
		return (oom_error());
	*dst = path;
	return (SUCCESS);
}
