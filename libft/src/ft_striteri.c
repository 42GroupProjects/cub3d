/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:59:10 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:20:55 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}
/*
// checked
// V3 checked
void shift_char(unsigned int i, char *c)
{
    (void)i;
    *c = *c - 1;
}
#include <stdio.h>
int	main(void)
{
	char	str[] = "J!offe!up!gjojti!MJCGU";
//	char	*str = NULL;

	ft_striteri(str, shift_char);
	printf("|%s|", str);
	return (0);
}*/
