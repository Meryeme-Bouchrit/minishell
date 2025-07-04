/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 12:09:35 by mbouchri          #+#    #+#             */
/*   Updated: 2024/11/20 11:14:56 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_get_length(long nb)
{
	int	len;

	len = 1;
	if (nb < 0)
	{
		len++;
		nb = -nb;
	}
	while (nb > 9)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	int		sign;
	long	num;

	num = n;
	i = ft_get_length(num);
	sign = 0;
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	str[i] = '\0';
	if (num < 0)
	{
		sign = 1;
		num = -num;
	}
	while (i-- > sign)
	{
		str[i] = (num % 10) + '0';
		num /= 10;
	}
	if (sign)
		str[0] = '-';
	return (str);
}
