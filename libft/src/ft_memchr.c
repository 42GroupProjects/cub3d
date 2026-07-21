/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:15:27 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:43:20 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n--)
	{
		if (*ptr == (unsigned char)c)
			return (ptr);
		ptr++;
	}
	return (NULL);
}

/*
// checked
// V3 checked
#include <stdio.h>
int main(void)
{
	char	*str = "Is t0here 1A difference?";
	int		c;
	size_t	n;

	c = 'X';
	n = 20;
	printf("|%p|", ft_memchr(str, c, n));
	return(0);
}*/
