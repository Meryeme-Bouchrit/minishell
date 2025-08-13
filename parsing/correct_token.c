/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correct_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:05:47 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/12 06:19:04 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	is_heredoc(t_token *tokens, const char *line, t_env *env)
{
	int	t;

	t = 0;
	while (tokens)
	{
		if (tokens && tokens->type == HEREDOC && tokens->next == NULL)
		{
			env->heredoc = true;
			while (line[t])
			{
				if (line[t] == '\'' || line[t] == '\"')
				{
					tokens->expand = true;
					break ;
				}
				t++;
			}
		}
		tokens = tokens->next;
	}
}

char	*token_next_string(t_token *tokens, const char *line, int *i,
		t_env *env)
{
	int		start;
	int		end;
	char	*result;
	t_token	*tmp;

	start = (*i);
	tmp = tokens;
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	env->heredoc = false;
	is_heredoc(tmp, line, env);
	end = end_len(line, (*i), env->heredoc);
	if (end == 0)
	{
		free_token_list(tokens);
		free(result);
		return (NULL);
	}
	while (start < end)
	{
		if (line[start] == '\"')
			result = my_strjoin(result, grep_doubleq(end, &start, line, env));
		else if (line[start] == '\'')
			result = my_strjoin(result, grep_singleq(end, &start, line));
		else
			result = my_strjoin(result, grep_no_quotes(i, end, &start, line,
						env));
	}
	env->heredoc = false;
	(*i) = start;
	return (result);
}

t_token	*tokenize(const char *line, t_env *env)
{
	int		i[2];
	char	*token_value;
	t_token	*tokens;

	i[0] = 0;
	i[1] = 0;
	tokens = NULL;
	while (line[i[0]])
	{
		i[1] = 0;
		if (line[i[0]] == '#')
			break ;
		while (ft_isspace(line[i[0]]))
			i[0]++;
		if (is_special(line[i[0]]) && line[i[0]])
		{
			if ((line[i[0]] == '<' && line[i[0] + 1] == '<')
				|| (line[i[0]] == '>' && line[i[0] + 1] == '>'))
				token_value = ft_substr(line, i[0]++, 2);
			else
				token_value = ft_substr(line, i[0], 1);
			i[0]++;
		}
		else
		{
			if (!line[i[0]])
				break ;
			i[1] = 2;
			token_value = token_next_string(tokens, line, i, env);
		}
		if (!token_value)
			return (NULL);
		if (token_value[0])
		{
			add_token(&tokens, ft_strdup(token_value), get_type(token_value, i[1]));		
			if (tokens->type == PIPE)
			{
				write(2, "minishell: syntax error near unexpected token `|'\n", 51);
				free_token_list(tokens);
				return (NULL);
			}
		}
		free(token_value);
		token_value = NULL;
	}
	if (check_for_syntax_error(tokens))
	{
		free_token_list(tokens);
		return (NULL);
	}
	return (tokens);
}
