/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 04:53:27 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/24 01:37:11 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	print_error_type(t_token_type type)
{
	if (type == REDIR_IN)
		write(2, "`<'\n", 4);
	if (type == REDIR_OUT)
		write(2, "`>'\n", 4);
	if (type == APPEND)
		write(2, "`>>'\n", 5);
	if (type == HEREDOC)
		write(2, "`<<'\n", 5);
	if (type == PIPE)
		write(2, "`|'\n", 4);
}

int	check_for_syntax_error(t_token *token)
{
	if (!token)
		return (0);
	while (token->next)
	{
		if (token->type != WORD && (token->next->type != WORD))
		{
			write(2, "minishell: syntax error near unexpected token ", 47);
			print_error_type(token->next->type);
			g_exit = 2;
			return (1);
		}
		token = token->next;
	}
	if (token->type != WORD || (my_strcmp(token->value, ";") == 0))
	{
		if (my_strcmp(token->value, ";") == 0)
			write(2, "minishell: syntax error near unexpected token `;'\n", 51);
		else
			write(2,
				"minishell: syntax error near unexpected token `newline'\n",
				57);
		g_exit = 2;
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
