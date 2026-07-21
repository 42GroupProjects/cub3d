/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 10:36:20 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 18:07:29 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	slen;
	char	*ptr;
	size_t	i;

	if (!s)
		return (NULL);
	slen = ft_strlen((char *)s);
	if (start > slen)
		return (ft_strdup(""));
	if (len > slen - start)
		len = slen - start;
	ptr = ft_calloc(len + 1, sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < len)
		ptr[i++] = s[start++];
	return (ptr);
}
/*
// checked for protection and valgrind
// V3 checked
#include <stdio.h>
int	main(void)
{
	char	*str = "I ll land in pace 24 o.O";
	size_t	i;
	size_t	len;
	char	*output;

	i = 5;
	len = 14;
	output = ft_substr(str, i , len);
	printf("|%s|", output);
	free(output);
	return (0);
}*/
