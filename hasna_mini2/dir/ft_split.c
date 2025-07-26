/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <zhassna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:15:15 by zhassna           #+#    #+#             */
/*   Updated: 2024/11/23 01:37:17 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count(char const *s, char c)
{
	size_t	count;
	size_t	p;

	count = 0;
	p = 0;
	while (*s)
	{
		count = 0;
		while (*s && *s == c)
			s++;
		while (*s && *s != c)
		{
			count++;
			s++;
		}
		if (count > 0)
			p++;
	}
	return (p);
}

static void	ft_free(char **str, size_t i)
{
	while (i > 0)
	{
		i--;
		free (str[i]);
	}
	free (str);
	return ;
}

static char	**ft_split_it(char const *s, char c, char **sp_str, size_t set)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (j < set)
	{
		while (*s == c)
			s++;
		i = 0;
		while (*s && (*s != c))
		{
			s++;
			i++;
		}
		sp_str[j] = malloc(sizeof(char) * (i + 1));
		if (!sp_str[j])
			return (ft_free(sp_str, j), NULL);
		ft_strlcpy(sp_str[j], s - i, i + 1);
		j++;
	}
	sp_str[j] = NULL;
	return (sp_str);
}

char	**ft_split(char const *s, char c)
{
	size_t	delimiter;
	char	**sp_str;

	if (!s)
		return (NULL);
	delimiter = count(s, c);
	sp_str = (char **) malloc(sizeof(char *) * (delimiter + 1));
	if (!sp_str)
		return (NULL);
	sp_str = ft_split_it(s, c, sp_str, delimiter);
	return (sp_str);
}
