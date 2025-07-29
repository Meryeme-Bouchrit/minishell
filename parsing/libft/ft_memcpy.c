/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <zhassna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 20:09:17 by zhassna           #+#    #+#             */
/*   Updated: 2024/11/23 03:29:19 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*ptr_dst;
	unsigned char	*ptr_src;

	if (!dst && !src)
		return (NULL);
	ptr_dst = (unsigned char *)dst;
	ptr_src = (unsigned char *)src;
	while (n--)
	{
		*ptr_dst++ = *ptr_src++;
	}
	return (dst);
}
