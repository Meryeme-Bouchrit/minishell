/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 04:53:27 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/20 01:04:18 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_for_syntax_error(t_token *token)
{
	if (!token)
		return (0);
	if (token && (token->type == PIPE))
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		return (0);
	}
	while (token->next)
	{
		if (token->type != WORD && (token->type == token->next->type))
		{
			write(2, "Error! syntax error near unexpected token `newline'\n",
				53);
			return (1);
		}
		token = token->next;
	}
	if (token->type != WORD)
	{
		write(2, "Error! syntax error near unexpected token `newline'\n", 53);
		return (1);
	}
	return (0);
}

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
					tokens->expand = false;
					break ;
				}
				t++;
			}
		}
		tokens = tokens->next;
	}
}
