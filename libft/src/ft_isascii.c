/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:36:10 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:34:39 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}
/*
//checked
//V3 checked and changed to <= 127 because of consistency
#include <stdio.h>
int	main(void)
{
	int		c;
	int		i;

	c = -1;
	i = ft_isascii(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 0;
	i = ft_isascii(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 127;
	i = ft_isascii(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 128;
	i = ft_isascii(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	return (0);
}*/
