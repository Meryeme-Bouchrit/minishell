/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correct_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:05:47 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/31 10:19:25 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*token_next_string(const char *line, int *i)
{
	int		start;
	int		end;
	char	*result;

	start = (*i);
	end = (*i);
	end = end_len(line, (*i));
	if (end == 0)
		return (NULL);
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	while (start < end)
	{
		if (line[start] == '\"')
			result = my_strjoin(result, grep_dq(end, &start, line));
		else if (line[start] == '\'')
			result = my_strjoin(result, grep_sq(end, &start, line));
		else
			result = my_strjoin(result, grep_no_quotes(end, &start, line));
	}
	(*i) = start;
	return (result);
}

t_token	*make_tokens(const char *line)
{
	int		i;
	char	*token_value;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (is_special(line[i]))
		{
			if ((line[i] == '<' && line[i + 1] == '<')
				|| (line[i] == '>' && line[i + 1] == '>'))
				token_value = ft_substr(line, i++, 2);
			else
				token_value = ft_substr(line, i, 1);
			i++;
		}
		else
			token_value = token_next_string(line, &i);
		if (!token_value || token_value[0] == '\0')
			break ;
		add_token(&tokens, ft_strdup(token_value), get_type(token_value));
	}
	return (tokens);
}
