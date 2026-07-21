/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:40:20 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:33:19 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
/*
//checked changed to 'A'
// V3 checked
#include <stdio.h>
int	main(void)
{
	char	c;
	int		i;

	c = 64;
	i = ft_isalpha(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 65;
	i = ft_isalpha(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 90;
	i = ft_isalpha(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 91;
	i = ft_isalpha(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	return (0);
}*/
