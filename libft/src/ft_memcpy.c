/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:06:01 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:53:52 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"	

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*ptrdest;
	const unsigned char	*ptrsrc;

	ptrdest = (unsigned char *)dest;
	ptrsrc = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		ptrdest[i] = ptrsrc[i];
		i++;
	}
	return (dest);
}
/*
// checked
// V3 checked
#include <stdio.h>
int main(void)
{
	char	str1[10] = "";
	char	*str2 = "Another one";
	size_t	n;
	void	*ptr;

	n = 15;
	ptr = ft_memcpy(str1, str2, n);
	printf("|%s|",(char *) ptr);
	return(0);
}*/
