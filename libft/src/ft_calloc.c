/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:43:44 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/21 13:32:15 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total;

	total = nmemb * size;
	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (total / nmemb != size)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}
/*
// checked changed from INT_MAX to max SIZETMAX stdint.h in libft.h
//V3 checked
#include <stdio.h>
int	main(void)
{
	int		i;
    char	*str = (char *)ft_calloc(10, sizeof(char));

	i = 0;
    if (str == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    printf("\nTest 2: String after calloc (zeros): ");
    while (i < 10)
	{
        printf("str[%d] = %d (ASCII value)\n", i, str[i]);
		i++;
    }

    void *empty = ft_calloc(0, sizeof(int));
    if (empty != NULL) {
        free(empty);
    }
    free(str);
    return 0;
}*/
