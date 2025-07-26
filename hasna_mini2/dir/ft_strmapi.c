/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:05:39 by zhassna           #+#    #+#             */
/*   Updated: 2024/11/18 15:00:47 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	unsigned int	len;
	char			*nstr;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	nstr = (char *) malloc(len + 1);
	if (!nstr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		nstr[i] = f(i, s[i]);
		i++;
	}
	nstr[len] = '\0';
	return (nstr);
}
