/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:20:14 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:32:50 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}
/*
// checked and changed from range (in 65 -90...) to functions
// V3 checked
#include <stdio.h>
int	main(void)
{
	char	c;
	int		i;

	c = 0;
	i = ft_isalnum(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 48;
	i = ft_isalnum(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 90;
	i = ft_isalnum(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	c = 91;
	i = ft_isalnum(c);
	printf("Testvalue:|%c|Returnvalue:|%d|\n", c, i);
	return (0);
}*/
