/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:28 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:03:11 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Print "Error\n<msg>" to stderr and return FAILURE, so a caller can write:
 *   return (parse_error(ERR_...));   // bad input / invalid map
 */
int parse_error(char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	return (FAILURE);
}

/**
 * Print the allocation-failure message and return OOM, kept distinct from
 * parse_error so the caller can tell "out of memory" from "bad input":
 *   return (oom_error());
 */
int oom_error(void)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(ERR_MALLOC, 2);
	ft_putchar_fd('\n', 2);
	return (OOM);
}

/** Advance past leading spaces/tabs and return the new position. */
char *skip_spaces(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

/**
 * TRUE if `line` begins with `id` followed by a space/tab. Guards against
 * "NORTH" being mistaken for the "NO" identifier.
 */
int match_id(char *line, char *id)
{
	int len;

	len = ft_strlen(id);
	if (ft_strncmp(line, id, len) != 0)
		return (FALSE);
	if (line[len] != ' ' && line[len] != '\t')
		return (FALSE);
	return (TRUE);
}

/** TRUE if `line` is empty or contains only whitespace. */
int is_blank(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (FALSE);
		i++;
	}
	return (TRUE);
}
