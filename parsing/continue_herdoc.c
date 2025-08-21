/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continue_herdoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:37:06 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/17 17:17:09 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*first_if_herdoc(int *start, const char *str)
{
	int	i;

	i = 0;
	if (empty_check((char *)str + i + 1))
		return (++(*start), ft_substr("", 0, 1));
	else if (str[i + 1] == '$')
		return (++(*start), ++(*start), ft_substr("", 0, 1));
	else
		return (++(*start), ft_substr(str, 0, 1));
}

char	*heredoc_expand_dollar(int *start, int end, const char *str, t_env *env)
{
	int		i;
	char	*var;
	char	*value;
	char	*result;

	i = 0;
	if (str[i] == '$' && (str[i + 1] == '\0' || str[i + 1] == ' ' || (*start
				+ 1) == end || empty_check((char *)str + i + 1) || str[i
				+ 1] == '$'))
		return (first_if_herdoc(start, str));
	if (str[++i] == '?')
		return (++(*start), ++(*start), ft_itoa(g_exit));
	if (str[i] >= '0' && str[i] <= '9')
		return (++(*start), ++(*start), ft_strdup(""));
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var = ft_substr(str, 1, i - 1);
	value = env_get(env, var);
	if (!value)
		value = "";
	result = ft_strdup(value);
	(*start) += i;
	return (free(var), result);
}

char	*expand_str(int end, int *start, const char *line, t_env *env)
{
	char	*result;
	char	*tmp;
	int		j;

	(void)env;
	j = 0;
	result = NULL;
	tmp = NULL;
	if (line[(*start)] == '$')
	{
		tmp = heredoc_expand_dollar(start, end, (line + (*start)), env);
		result = my_strjoin(result, tmp);
	}
	j = (*start);
	while ((*start) < end && line[(*start)] && line[(*start)] != '$')
		(*start)++;
	result = my_strjoin(result, ft_substr(line, j, (*start) - j));
	return (result);
}

char	*expand_variables(const char *line, t_env *env)
{
	int		start;
	int		end;
	char	*result;

	start = 0;
	end = ft_strlen(line);
	if (end == 0)
		return (NULL);
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	while (start < end)
	{
		result = my_strjoin(result, expand_str(end, &start, line, env));
	}
	return (result);
}
