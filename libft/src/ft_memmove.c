/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:16:30 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:00:32 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*ptrdest;
	const unsigned char	*ptrsrc;
	size_t				i;

	ptrdest = (unsigned char *)dest;
	ptrsrc = (const unsigned char *) src;
	i = 0;
	if (dest < src)
	{
		while (i < n)
		{
			ptrdest[i] = ptrsrc[i];
			i++;
		}
	}
	else
	{
		while (i < n)
		{
			n--;
			ptrdest[n] = ptrsrc[n];
		}
	}
	return (dest);
}
/*
// checked - changed to original func
// V3 checked 
#include <stdio.h>
#include <string.h>
int main(void)
{
	char	*s = strdup(" I want to be copied. ");
	char	d[15];
	size_t	t;

	t = 155;
	printf(":%p:\n", (char *)ft_memmove(s, d, t));
	printf(":%p:\n", (char *)memmove(s, d, t));
	free(s);
	return(0);
}*/
