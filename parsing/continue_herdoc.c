/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continue_herdoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 23:37:06 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/08 00:33:05 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*expand_str(int end, int *start, const char *line, t_env *env)
{
	char	*result;
	char	*tmp;
	int		j;

	j = 0;
	result = NULL;
	tmp = NULL;
	if (line[(*start)] == '$')
	{
		tmp = secnd_expand_dollar(start, end, (line + (*start)), env);
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
	printf("result=[%s]", result);
	return (result);
}
