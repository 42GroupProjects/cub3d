/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:56:16 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/20 18:21:34 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*cpys;
	size_t	i;

	i = 0;
	cpys = (char *)ft_calloc(ft_strlen((char *)s) + 1, 1);
	if (!cpys)
		return (NULL);
	while (s[i])
	{
		cpys[i] = s[i];
		i++;
	}
	return (cpys);
}
/*
//v3 changed to size_t and removed nullterminating
#include <stdio.h>
int	main(void)
{
	char	*str1 = "Oh no. Where is Giti?!";
	char	*str2;

	str2 = ft_strdup(str1);
	printf("|%s|", str2);
	return (0);	
}*/
