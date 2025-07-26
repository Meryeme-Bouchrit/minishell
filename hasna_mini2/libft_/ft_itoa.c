/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 21:54:59 by zhassna           #+#    #+#             */
/*   Updated: 2024/11/18 16:18:20 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_convert(char *str, long nb, int i, int sign)
{
	str[i] = '\0';
	i--;
	if (nb == 0)
		str[i] = nb + '0';
	while (nb > 0)
	{
		str[i] = '0' + (nb % 10);
		nb = nb / 10;
		i--;
	}
	if (sign == -1)
		str[0] = '-';
	return (str);
}

static int	ft_size(int n)
{
	int		i;
	long	nb;

	nb = n;
	i = 0;
	if (nb <= 0)
	{
		nb = -nb;
		i++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		sign;
	long	nb;
	int		i;

	nb = n;
	sign = 1;
	i = ft_size(nb);
	res = malloc(i + 1);
	if (!res)
		return (NULL);
	if (nb < 0)
	{
		nb = -nb;
		sign = -1;
	}
	ft_convert(res, nb, i, sign);
	return (res);
}
