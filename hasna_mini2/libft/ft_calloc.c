/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <zhassna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 23:37:39 by zhassna           #+#    #+#             */
/*   Updated: 2024/11/23 02:38:11 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	n;
	void	*calo;

	if (count && size && count > SIZE_MAX / size)
		return (NULL);
	calo = malloc(count * size);
	if (!calo)
		return (NULL);
	n = count * size;
	ft_bzero(calo, n);
	return ((void *)calo);
}
