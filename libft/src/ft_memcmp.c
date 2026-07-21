/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:48:35 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:44:13 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (n--)
	{
		if ((*(unsigned char *)s1) != (*(unsigned char *)s2))
		{
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		}
		s1++;
		s2++;
	}
	return (0);
}
/*
// checked
// V3 checked
#include <stdio.h>
int	main(void)
{
	char	*str1 = "Is there A difference?";
	char	*str2 = "Is there a difference?";
	size_t	i;

	i = 0;
	printf("%d\n", ft_memcmp(str1, str2, i));
	return(0);
}*/
