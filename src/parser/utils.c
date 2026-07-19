/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 19:02:28 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 19:14:04 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_error(char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	return (FAILURE);
}

int	oom_error(void)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(ERR_MALLOC, 2);
	ft_putchar_fd('\n', 2);
	return (OOM);
}

char	*skip_spaces(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

int	match_id(char *line, char *id)
{
	int	len;

	len = ft_strlen(id);
	if (ft_strncmp(line, id, len) != 0)
		return (FALSE);
	if (line[len] != ' ' && line[len] != '\t')
		return (FALSE);
	return (TRUE);
}

int	is_blank(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (FALSE);
		i++;
	}
	return (TRUE);
}
