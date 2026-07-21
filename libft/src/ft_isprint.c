/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:26:23 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:37:00 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c < 127)
		return (1);
	return (0);
}
/*
//checked
// V3 checked
#include <stdio.h>
int	main(void)
{
	int		c;
	int		i;

	c = 31;
	i = ft_isprint(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 32;
	i = ft_isprint(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 126;
	i = ft_isprint(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 127;
	i = ft_isprint(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	return (0);
}*/
