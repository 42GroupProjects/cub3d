/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:21:44 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:28:47 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	l_len;

	if (*little == '\0')
		return ((char *)big);
	l_len = ft_strlen((char *)little);
	if (len == 0 || l_len > len)
		return (NULL);
	i = 0;
	while (big[i] && i + l_len <= len)
	{
		j = 0;
		while (big[i + j] == little[j] && little[j])
			j++;
		if (j == l_len)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}
/*
// checked included the strnstr compare. cc with -lbsd
// V3 checked
#include <stdio.h>
#include <bsd/string.h>
int	main(void)
{
	char	*str1 = "Dont fool me";
	char	*str2 = "fool";
	size_t	i;

	i = 8;
	printf("Return|%s|\n", ft_strnstr(str1, str2, i));
	printf("Return|%s\n", strnstr(str1, str2, i));
	i = 9;
	printf("Return|%s|\n", ft_strnstr(str1, str2, i));
	printf("Return|%s|\n", strnstr(str1, str2, i));
 	str2 = "full";
	printf("Return|%s|\n", ft_strnstr(str1, str2, i));
	printf("Return|%s|\n", strnstr(str1, str2, i));
	str2 = "";
	printf("Return|%s|\n", ft_strnstr(str1, str2, i));
	printf("Return|%s|\n", strnstr(str1, str2, i));
	str2 = NULL;
	printf("Return|%s|\n", ft_strnstr(str1, str2, i));
	printf("Return|%s|\n", strnstr(str1, str2, i));
	return(0);
}*/
