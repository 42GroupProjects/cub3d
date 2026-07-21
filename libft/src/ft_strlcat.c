/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:54:20 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:26:06 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;
	size_t	dst_len;

	i = 0;
	src_len = ft_strlen((char *)src);
	dst_len = ft_strlen((char *)dst);
	if (size == 0)
		return (src_len);
	if (size <= dst_len)
		return (size + src_len);
	while (src[i] && (dst_len + i) < size - 1)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}
/*
// checked use -lbsd to cc
// V3 checked
#include <stdio.h>
#include <bsd/string.h>
int	main(void)
{
	char	*str = NULL;
	char	dst[25] = "i";
	char	dst1[25] = "i";	
	size_t	i;

	i = 25;
	printf("|%zu||%s|\n", ft_strlcat(dst, str, i), dst);
	printf("|%zu||%s|\n", strlcat(dst1, str, i), dst1);
	return (0);
}*/
