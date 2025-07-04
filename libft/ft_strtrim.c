/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:41:13 by mbouchri          #+#    #+#             */
/*   Updated: 2024/11/16 19:35:33 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	first;
	size_t	last;
	size_t	i;
	char	*trim_str;

	first = 0;
	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[first] != '\0' && ft_strchr(set, s1[first]))
		first++;
	last = ft_strlen(s1);
	while (last > first && ft_strchr(set, s1[last - 1]))
		last--;
	trim_str = (char *)malloc(last - first + 1);
	if (!trim_str)
		return (NULL);
	while (i < (last - first))
	{
		trim_str[i] = s1[first + i];
		i++;
	}
	trim_str[i] = '\0';
	return (trim_str);
}
