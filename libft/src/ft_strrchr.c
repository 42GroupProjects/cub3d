/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:03:39 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:29:23 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	i = ft_strlen((char *)s);
	while (i > 0)
	{
		if (s[i] == (unsigned char)c)
			return ((char *)(s + i));
		i--;
	}
	if (s[0] == (unsigned char)c)
		return ((char *)(s + i));
	return (NULL);
}
/*
// checked changed int to size_t compared to strrchr
// V3 checked
#include <stdio.h>
#include <bsd/string.h>
int main(void)
{
	char	*str = "tripouille";
	int		i;
	char	*output;

	i = 't';
	output = ft_strrchr(str, i);
	printf("Input|%c|Output|%s|\n", i, output);
	printf("Input|%c|Output|%s|\n", i, strrchr(str, i));
	return(0);
}*/
