/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:27:51 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/06 03:18:08 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_symbols(char c)
{
	return (c == '$' || c == '<' || c == '>' || c == '|' || c == '\0'
		|| c == '\'' || c == '\"');
}

int	empty_check(char *s)
{
	int	len;
	int	i;

	len = ft_strlen(s);
	if (len == 2 && ((s[0] == '\"' && s[len - 1] == '\"') || (s[0] == '\''
				&& s[len - 1] == '\'')))
		return (1);
	else if ((s[0] == '\"' && s[len - 1] == '\"') || (s[0] == '\'' && s[len
			- 1] == '\''))
	{
		s++;
		len -= 2;
	}
	i = 0;
	while (s[i] && i < len)
	{
		if (s[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

char	*secnd_expand_dollar(int *start, const char *str, t_env *env)
{
	int		i;
	char	*var;
	char	*value;
	char	*result;
	int		vlen;

	i = *start;
	i = 0;
	vlen = 0;
	if (str[i] == '$' && (str[i + 1] == '\0' || str[i + 1] == ' '
			|| empty_check((char *)str + 1)))
	{
		if (empty_check((char *)str + 1))
			return (ft_substr("", 0, 1));
		else
			return (ft_substr(str, 0, 1));
	}
	////////////     side (2)    ///////////
	i++; // skip $ sign
	if (str[i] == '?' || (str[i] >= '0' && str[i] <= '9'))
		i++;
	else
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
			i++;
	var = ft_substr(str, 1, i - 1);
	value = get_env_value(env, var);
	// getenv(var); // get variable from env
	if (!value)
		value = ""; // if var doesn't exist
	result = ft_strdup(value);
	(*start) += i;
	// printf("dollar_result= [%s] && dollar_var=[%s]\n", result, var);
	free(var);
	return (result);
}
