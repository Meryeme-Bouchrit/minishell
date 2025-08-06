/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correct_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:05:47 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/06 03:48:54 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*token_next_string(t_token *tokens, const char *line, int *i,
		t_env *env)
{
	int		start;
	int		end;
	char	*result;

	start = (*i);
	end = (*i);
	end = end_len(line, (*i));
	if (end == 0)
	{
		free_token_list(tokens);
		return (NULL);
	}
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	while (start < end)
	{
		if (line[start] == '\"')
			result = my_strjoin(result, grep_doubleq(tokens, end, &start, line,
						env));
		else if (line[start] == '\'')
			result = my_strjoin(result, grep_singleq(end, &start, line));
		else
			result = my_strjoin(result, grep_no_quotes(tokens, end, &start,
						line, env));
	}
	(*i) = start;
	return (result);
}

t_token	*tokenize(const char *line, t_env *env)
{
	int		i;
	char	*token_value;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		if (line[i] == '#')
			break ;
		while (ft_isspace(line[i]))
			i++;
		if (is_special(line[i]) && line[i])
		{
			if ((line[i] == '<' && line[i + 1] == '<') || (line[i] == '>'
					&& line[i + 1] == '>'))
				token_value = ft_substr(line, i++, 2);
			else
				token_value = ft_substr(line, i, 1);
			i++;
		}
		else
		{
			if (!line[i])
				break ;
			token_value = token_next_string(tokens, line, &i, env);
		}
		if (!token_value)
			return (NULL);
		add_token(&tokens, ft_strdup(token_value), get_type(token_value));
		free(token_value);
		token_value = NULL;
	}
	// check_for_error_syntax(t_token *token);
	if (check_for_syntax_error(tokens))
		return (NULL);
	// zhassna@c4r7p1:~/Desktop/a_minishell_team/mini_v2$ >>>
	// bash: syntax error near unexpected token `>'
	return (tokens);
}
