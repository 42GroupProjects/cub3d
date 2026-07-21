/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:56:55 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/21 13:30:15 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fillbufferuntilnewline(char **save, int fd)
{
	ssize_t	bytes_read;
	char	*tmp;
	char	*buffer;

	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	while (1)
	{
		if ((*save) && ft_strchr(*save, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!*save)
			*save = ft_strdup("");
		tmp = ft_strjoin(*save, buffer);
		free(*save);
		*save = tmp;
	}
	free(buffer);
}

static char	*getoutput(char *save)
{
	size_t	i;
	size_t	j;
	char	*output;

	if (!save || !*save)
		return (NULL);
	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (save[i] == '\n')
		i++;
	output = ft_calloc(i + 1, sizeof(char));
	if (!output)
		return (NULL);
	j = 0;
	while (j < i)
	{
		output[j] = save[j];
		j++;
	}
	return (output);
}

static void	getsave(char **save)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while ((*save)[i] && (*save)[i] != '\n')
		i++;
	if ((*save)[i] == '\n')
		i++;
	tmp = ft_calloc(ft_strlen(*save + i) + 1, sizeof(char));
	if (!tmp)
		return ;
	j = 0;
	while ((*save)[i])
		tmp[j++] = (*save)[i++];
	free(*save);
	*save = tmp;
}

char	*get_next_line(int fd)
{
	static char	*save = NULL;
	char		*output;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(save);
		save = NULL;
		return (NULL);
	}
	fillbufferuntilnewline(&save, fd);
	output = getoutput(save);
	if (!output)
	{
		free(save);
		save = NULL;
		return (NULL);
	}
	getsave(&save);
	return (output);
}
