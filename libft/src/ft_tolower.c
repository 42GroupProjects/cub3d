/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:37:29 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 18:08:57 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}
/*
// checked
// V3 checked
#include <stdio.h>
int	main(void)
{
	int		c;
	int		i;

	c = 65;
	i = ft_tolower(c);
	printf("Testvalue:|%c|Returnvalue:|%c|\n", c, i);
	return (0);
}*/
