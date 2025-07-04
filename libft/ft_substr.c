/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:41:52 by mbouchri          #+#    #+#             */
/*   Updated: 2024/11/20 10:47:39 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t			i;
	char			*sub;
	unsigned int	j;

	if (!s)
		return (NULL);
	j = ft_strlen(s);
	if (start > j)
		return (ft_strdup(""));
	if (j - start > len)
		sub = malloc((len) + 1);
	else
		sub = malloc((j - start) + 1);
	if (!sub)
		return (NULL);
	i = 0;
	while (s[i + start] && i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
