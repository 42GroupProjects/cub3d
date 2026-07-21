/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:51:30 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:03:29 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)s)[i] = (char)c;
		i++;
	}
	return ((void *)s);
}
/*
//checked
// V3 checked
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char	str[25] = "Why is Drucilla leaving?";
	int		i;
	int		len;

	i = 65;
	len = 15;
	ft_memset(str, i, len);
	printf("|%s|\n", str);
	memset(str, 97 , len);
	printf("|%s|\n", str);
	return (0);
}*/
