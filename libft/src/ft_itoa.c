/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:44:22 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:42:02 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	countdigits(long n)
{
	size_t	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static size_t	checknegative(long n)
{
	size_t	neg;

	neg = 0;
	if (n < 0)
	{
		neg = 1;
	}
	return (neg);
}

char	*ft_itoa(int n)
{
	char	*out;
	size_t	count;
	long	nb;
	size_t	is_negative;

	nb = (long) n;
	is_negative = checknegative(nb);
	if (is_negative)
		nb = -nb;
	count = countdigits(nb) + is_negative;
	out = ft_calloc((count + 1), 1);
	if (!out)
		return (NULL);
	if (nb == 0)
		out[0] = '0';
	while (nb)
	{
		out[--count] = ((nb % 10) + '0');
		nb /= 10;
	}
	if (is_negative)
		out[0] = '-';
	return (out);
}
/*
// checked - changed from int to size_t
// V3 checked leave it like this for now. might rebuild it.
#include <stdio.h>
int	main(void)
{
	int		i;
	char	*test;
	i = -2147483648;
	test = ft_itoa(i);
	printf("%s", test);
	free(test);
	return(0);
}*/
