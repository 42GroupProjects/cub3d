/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:12 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 18:45:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	count_words(char **words)
{
	int	n;

	n = 0;
	while (words[n])
		n++;
	return (n);
}

static int	is_valid_byte(char *s)
{
	int	i;

	s = skip_spaces(s);
	if (!s[0])
		return (FALSE);
	i = 0;
	while (s[i] && s[i] != ' ' && s[i] != '\t')
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (FALSE);
		i++;
	}
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	if (s[i] != '\0')
		return (FALSE);
	if (ft_atoi(s) < 0 || ft_atoi(s) > RGB_MAX)
		return (FALSE);
	return (TRUE);
}

static int	fill_rgb(int *rgb, char **parts)
{
	int	i;

	if (count_words(parts) != RGB_COUNT)
		return (FALSE);
	i = 0;
	while (i < RGB_COUNT)
	{
		if (!is_valid_byte(parts[i]))
			return (FALSE);
		rgb[i] = ft_atoi(skip_spaces(parts[i]));
		i++;
	}
	return (TRUE);
}

static int	count_commas(char *s)
{
	int	n;

	n = 0;
	while (*s)
	{
		if (*s == ',')
			n++;
		s++;
	}
	return (n);
}

int	set_color(int *rgb, char *value, int *flag)
{
	char	**parts;

	if (*flag)
		return (parse_error(ERR_DUP_COLOR));
	value = skip_spaces(value);
	if (count_commas(value) != RGB_COUNT - 1)
		return (parse_error(ERR_BAD_COLOR));
	parts = ft_split(value, ',');
	if (!parts)
		return (oom_error());
	if (!fill_rgb(rgb, parts))
	{
		free_strarr(&parts);
		return (parse_error(ERR_BAD_COLOR));
	}
	free_strarr(&parts);
	*flag = TRUE;
	return (SUCCESS);
}
