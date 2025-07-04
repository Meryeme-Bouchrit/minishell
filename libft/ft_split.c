/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 12:53:41 by mbouchri          #+#    #+#             */
/*   Updated: 2024/11/20 12:10:30 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_word(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i] != c && str[i] != '\0')
		{
			count++;
			while (str[i] != c && str[i] != '\0')
				i++;
		}
	}
	return (count);
}

static int	word_len(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
		i++;
	return (i);
}

static char	**free_split(char **splited, int j)
{
	while (j >= 0)
	{
		free(splited[j]);
		j--;
	}
	free(splited);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		words_num;
	char	**splited;
	int		j;

	if (!s)
		return (NULL);
	splited = malloc((count_word((char *)s, c) + 1) * sizeof(char *));
	if (!splited)
		return (NULL);
	words_num = count_word((char *)s, c);
	j = 0;
	while (j < words_num)
	{
		while (*s == c)
			s++;
		splited[j] = ft_substr(s, 0, word_len((char *)s, c));
		if (!splited[j])
			return (free_split(splited, j));
		s += word_len((char *)s, c);
		j++;
	}
	splited[j] = NULL;
	return (splited);
}
