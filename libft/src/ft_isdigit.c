/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:45:46 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:35:29 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
/*
//checked
// V3 checked removed the brackets - cons
#include <stdio.h>
int	main(void)
{
	char	c;
	int		i;

	c = 47;
	i = ft_isdigit(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 48;
	i = ft_isdigit(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 57;
	i = ft_isdigit(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 58;
	i = ft_isdigit(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	return (0);
}*/
