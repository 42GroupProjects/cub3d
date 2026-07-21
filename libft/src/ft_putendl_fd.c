/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:59:59 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:06:34 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}
/*
// checked - changed while loop to write+strlen slow sysfunc
// V3 checked removed size_t i from previous version
#include <stdio.h>
int	main(void)
{
	char	*str = NULL;
	int		i;

	i = 1;
	ft_putendl_fd(str, i);
	return (0);
}*/
