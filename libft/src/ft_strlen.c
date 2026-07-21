/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:30:31 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 14:27:11 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
/*
// checked changed int to size_t
// V3 checked
#include <stdio.h>
#include <string.h>
int main(void)
{
	char	*str = "finishing touch";
	size_t	count;

	count = ft_strlen(str);
	printf("Der String:|%s| ist |%lu| Zeichen lang\n", str, count);
	printf("Der String:|%s| ist |%lu| Zeichen lang\n", str, strlen(str));
	return(0);
}*/
