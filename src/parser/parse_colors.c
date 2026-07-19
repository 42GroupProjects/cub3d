/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:12 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:03:06 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/** Count entries in a NULL-terminated array. */
static int count_words(char **words)
{
	int n;

	n = 0;
	while (words[n])
		n++;
	return (n);
}

/** TRUE if `s` is a non-empty all-digit value in [0, RGB_MAX]. */
static int is_valid_byte(char *s)
{
	int i;

	if (!s[0])
		return (FALSE);
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (FALSE);
		i++;
	}
	if (ft_atoi(s) < 0 || ft_atoi(s) > RGB_MAX)
		return (FALSE);
	return (TRUE);
}

/** TRUE when `parts` holds exactly RGB_COUNT valid bytes; stores into rgb. */
static int fill_rgb(int *rgb, char **parts)
{
	int i;

	if (count_words(parts) != RGB_COUNT)
		return (FALSE);
	i = 0;
	while (i < RGB_COUNT)
	{
		if (!is_valid_byte(parts[i]))
			return (FALSE);
		rgb[i] = ft_atoi(parts[i]);
		i++;
	}
	return (TRUE);
}

/**
 * Parse "R,G,B" from `value` into rgb; *flag guards against a duplicate F/C.
 * Returns SUCCESS, FAILURE (duplicate / invalid color) or OOM (split failed).
 */
int set_color(int *rgb, char *value, int *flag)
{
	char **parts;

	if (*flag)
		return (parse_error(ERR_DUP_COLOR));
	value = skip_spaces(value);
	parts = ft_split(value, ',');
	if (!parts)
		return (oom_error());
	if (!fill_rgb(rgb, parts))
		return (free_strarr(&parts), parse_error(ERR_BAD_COLOR));
	free_strarr(&parts);
	*flag = TRUE;
	return (SUCCESS);
}
