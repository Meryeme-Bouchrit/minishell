/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:27:51 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/21 02:44:17 by zhassna          ###   ########.fr       */
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
	if (i == 0)
		return (0);
	return (1);
}

char	*first_if(t_ctx *ctx, const char *str)
{
	int	i;

	i = 0;
	if (empty_check((char *)str + i + 1))
		return (++(ctx->start), ft_substr("", 0, 1));
	else if (str[i + 1] == '$')
		return (++(ctx->start), ++(ctx->start), ft_substr("", 0, 1));
	else
		return (++(ctx->start), ft_substr(str, 0, 1));
}

char	*secnd_expand_dollar(t_ctx *ctx, int end, const char *str, t_env *env)
{
	int		i;
	char	*var;
	char	*value;
	char	*result;

	i = 0;
	if (str[i] == '$' && (str[i + 1] == '\0' || str[i + 1] == ' ' || (ctx->start
				+ 1) == end || empty_check((char *)str + i + 1) || str[i
				+ 1] == '$'))
		return (first_if(ctx, str));
	if (str[++i] == '?')
		return (++(ctx->start), ++(ctx->start), ft_itoa(g_exit));
	if (str[i] >= '0' && str[i] <= '9')
		return (++(ctx->start), ++(ctx->start), ft_strdup(""));
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var = ft_substr(str, 1, i - 1);
	value = env_get(env, var);
	if (!value)
		value = "";
	result = ft_strdup(value);
	(ctx->start) += i;
	return (free(var), result);
}
