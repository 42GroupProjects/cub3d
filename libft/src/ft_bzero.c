/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:49:40 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:31:18 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)s)[i] = 0;
		i++;
	}
}
/*
//checked
//V3 checked
#include <stdio.h>
int main(void)
{
	char	str[6] = "Hello";
	size_t	i;

	i = 3;
	ft_bzero(str, i);
	printf("|%s|", str);
	return(0);
}*/
