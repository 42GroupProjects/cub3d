/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:35:12 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 18:09:18 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
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
	i = ft_toupper(c);
	printf("Testvalue:|%c|Returnvalue:|%c|\n", c, i);
	c = 97;
	i = ft_toupper(c);
	printf("Testvalue:|%c|Returnvalue:|%c|\n", c, i);
	return (0);
}*/
