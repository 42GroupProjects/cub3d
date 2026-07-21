/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:26:54 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:12:21 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}
/*
//v2 checked changed to ft_strlen no whileloop
//v3 rm leftovers from old version
int	main(void)
{
	char	*str = "I m good at hiding.";
	int		i;

	i = 1;
	ft_putstr_fd(str, i);
	return (0);
}*/
