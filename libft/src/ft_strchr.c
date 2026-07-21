/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:57:28 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:19:05 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (unsigned char)c)
		return ((char *)&s[i]);
	return (NULL);
}
/*
// checked
// V3 checked and changed int to size_t
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char	*str = "tri pouille";
	int		c;
	char	*output;

	c = ' ';
	output = ft_strchr(str, c);
	printf("Input|%c|Output|%s|\n", c, output);
	printf("Input|%c|Output|%s|\n", c, strchr(str, c));
	return (0);
}*/
