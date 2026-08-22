/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:46:57 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/24 21:45:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	clear_backup(char **backup)
{
	if (backup && *backup)
	{
		free(*backup);
		*backup = NULL;
	}
}

static char	*read_line(int fd, char *buffer, char **backup)
{
	int		check;
	char	*temp;

	check = 1;
	while (check)
	{
		check = read(fd, buffer, BUFFER_SIZE);
		if (check == -1)
			return (clear_backup(backup), NULL);
		else if (check == 0)
			break ;
		buffer[check] = '\0';
		if (!*backup)
			*backup = ft_strdup("");
		if (!*backup)
			return (NULL);
		temp = *backup;
		*backup = ft_strjoin(temp, buffer);
		free(temp);
		if (!*backup)
			return (NULL);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (*backup);
}

static char	*extract(char *line)
{
	int		i;
	char	*temp;

	i = 0;
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	if (line[i] == '\0')
		return (NULL);
	temp = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (!temp)
		return (NULL);
	if (temp[0] == '\0')
	{
		free(temp);
		return (NULL);
	}
	line[i + 1] = '\0';
	return (temp);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	static char	*backup;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (clear_backup(&backup), NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (clear_backup(&backup), NULL);
	line = read_line(fd, buffer, &backup);
	free(buffer);
	if (!line)
		return (NULL);
	backup = extract(line);
	return (line);
}
