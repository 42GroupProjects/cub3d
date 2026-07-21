/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:58:53 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/20 18:23:02 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	char	*output;

	if (!s)
		return (0);
	output = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!output)
		return (0);
	i = 0;
	if (output == 0)
		return (NULL);
	while (s[i] != '\0')
	{
		output[i] = (*f)(i, s[i]);
		i++;
	}
	return (output);
}
/*
//V2 checked changed int i to size_t i
//V3 rm nullterminating because calloc!
#include <stdio.h>
char shift_char(unsigned int i, char c)
{
    return c + i;
}

int main()
{
    char *str = NULL;
    char *result;

	result = ft_strmapi(str, shift_char);
    printf("Transformed string: %s\n", result);
    free(result);
	return 0;
}*/
