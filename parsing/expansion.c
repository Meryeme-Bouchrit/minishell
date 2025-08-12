/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:27:51 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/08 04:57:53 by zhassna          ###   ########.fr       */
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
	else if ((s[0] == '\"' && s[len - 1] == '\"')
		|| (s[0] == '\'' && s[len - 1] == '\''))
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
	if (i == 0)
		return (0);
	return (1);
}

char	*secnd_expand_dollar(int *start, int end, const char *str, t_env *env)
{
	int		i;
	char	*var;
	char	*value;
	char	*result;

	i = 0;
	if (str[i] == '$' && (str[i + 1] == '\0' || str[i + 1] == ' ' || (*start
				+ 1) == end || empty_check((char *)str + i + 1)))
	{
		if (empty_check((char *)str + i + 1))
			return (++(*start), ft_substr("", 0, 1));
		else
			return (++(*start), ft_substr(str, 0, 1));
	}
	if (str[++i] == '?')
		return (++(*start), ++(*start), ft_itoa(exit_status));
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var = ft_substr(str, 1, i - 1);
	value = get_env_value(env, var);
	if (!value)
		value = "";
	result = ft_strdup(value);
	(*start) += i;
	return (free(var), result);
}
