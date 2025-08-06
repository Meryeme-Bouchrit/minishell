/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <zhassna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 22:47:00 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/05 01:40:57 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static char	*join_str(char const *s1, char const *s2, char *str)
{
	size_t	i;

	i = 0;
	if (s1)
	{
		while (*s1)
		{
			str[i] = *s1;
			i++;
			s1++;
		}
	}
	if (s2)
	{
		while (*s2)
		{
			str[i] = *s2;
			i++;
			s2++;
		}
	}
	str[i] = '\0';
	return (str);
}

char	*my_strjoin(char *s1, char *s2)
{
	size_t	s1len;
	size_t	s2len;
	char	*str;

	if (!s1 && !s2)
		return (NULL);
	s1len = 0;
	s2len = 0;
	if (s1)
		s1len = ft_strlen(s1);
	if (s2)
		s2len = ft_strlen(s2);
	str = (char *)malloc(((s1len + s2len) * sizeof(char)) + 1);
	if (!str)
		return (NULL);
	str = join_str(s1, s2, str);
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (str);
}
