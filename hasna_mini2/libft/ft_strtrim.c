/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <zhassna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:43:12 by zhassna           #+#    #+#             */
/*   Updated: 2024/11/20 01:26:57 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_copy(char *copy, const char *s1, size_t start, size_t end)
{
	size_t	i;

	i = 0;
	while (start < end)
	{
		copy[i] = s1[start];
		start++;
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*copy;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (start < end && ft_strchr(set, s1[end - 1]))
		end--;
	copy = (char *) malloc(sizeof(char) * (end - start + 1));
	if (!copy)
		return (NULL);
	copy = ft_copy(copy, s1, start, end);
	return (copy);
}
