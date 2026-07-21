/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:17:51 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/20 18:35:15 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (unsigned char)s1[i] == (unsigned char)s2[i])
	{
		i++;
	}
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
/*
//V2 checked changed all to unsigned char added strncmp
//V3 removed unnecessary code
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char	*str1 = "aZ";
	char	*str2 = "a";
	size_t	i;

	i = 2;
	printf("Mine: |%d|\n", ft_strncmp(str1, str2, i));
	printf("Orig: |%d|\n", strncmp(str1, str2, i));
	return 0;
}*/
