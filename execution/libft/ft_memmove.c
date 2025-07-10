/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:08:02 by mbouchri          #+#    #+#             */
/*   Updated: 2024/11/17 19:32:30 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dst1;
	unsigned char	*src1;

	dst1 = (unsigned char *)dst;
	src1 = (unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	if (n == 0)
		return (dst);
	if (dst1 > src1)
	{
		while (n--)
			dst1[n] = src1[n];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			dst1[i] = src1[i];
			i++;
		}
	}
	return (dst);
}
