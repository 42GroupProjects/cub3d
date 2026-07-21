/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:39:54 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:26:43 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = 0;
	src_len = ft_strlen(src);
	if (size != 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (src_len);
}
/*
// checked -lbsd 
// V3 checked
#include <stdio.h>
#include <bsd/string.h>
int main(void)
{
	char		dst[5];
	char		dst1[5];
	const char	*str1 = NULL;
	size_t		i;

	i = 7;
	printf("|%zu|", ft_strlcpy(dst, str1, i));
	printf("|%zu|", strlcpy(dst1, str1, i));
	return(0);
}*/
