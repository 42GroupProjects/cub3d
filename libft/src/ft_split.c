/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:57:57 by lwittwer          #+#    #+#             */
/*   Updated: 2025/05/22 16:43:39 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	countwords(char *s, char c)
{
	size_t	i;
	size_t	check;
	size_t	count;

	check = 0;
	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && check == 0)
		{
			check = 1;
			count++;
		}
		else if (s[i] == c)
			check = 0;
		i++;
	}
	return (count);
}

static void	freearr(char **arr, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	*createnfillarr(const char *str, size_t start, size_t end)
{
	char	*onearr;
	size_t	i;

	i = 0;
	onearr = ft_calloc((end - start + 1), sizeof(char));
	if (!onearr)
		return (NULL);
	while (start < end)
	{
		onearr[i++] = str[start++];
	}
	return (onearr);
}

static char	**findstartend(const char *str, char **arr, char c, size_t words)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	k = 0;
	while (str[i] && k < words)
	{
		while (str[i] && str[i] == c)
			i++;
		j = i;
		while (str[j] && str[j] != c)
			j++;
		if (j > i)
		{
			arr[k] = createnfillarr(str, i, j);
			if (!arr[k])
				return (freearr(arr, k), NULL);
			k++;
		}
		i = j;
	}
	arr[k] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**arr;

	if (!s)
		return (NULL);
	words = countwords((char *)s, c);
	arr = ft_calloc(words + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	if (!findstartend(s, arr, c, words))
		return (NULL);
	return (arr);
}

