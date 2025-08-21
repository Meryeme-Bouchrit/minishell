/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:24:13 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/08 03:58:22 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token_type	get_type(char *s, int a)
{
	if (a == 1)
		return (SPLIT);
	if (a == 2 || a == 4)
		return (WORD);
	if (my_strcmp(s, "|") == 0)
		return (PIPE);
	if (my_strcmp(s, "<") == 0)
		return (REDIR_IN);
	if (my_strcmp(s, ">") == 0)
		return (REDIR_OUT);
	if (my_strcmp(s, ">>") == 0)
		return (APPEND);
	if (my_strcmp(s, "<<") == 0)
		return (HEREDOC);
	return (WORD);
}

bool	redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == APPEND
		|| type == HEREDOC);
}

bool	is_redirection(char *s)
{
	if (my_strcmp(s, "<") == 0)
		return (1);
	if (my_strcmp(s, ">") == 0)
		return (1);
	if (my_strcmp(s, ">>") == 0)
		return (1);
	if (my_strcmp(s, "<<") == 0)
		return (1);
	return (0);
}

t_redirections	get_redirect_type(char *s)
{
	if (my_strcmp(s, "<") == 0)
		return (T_REDIR_IN);
	if (my_strcmp(s, ">") == 0)
		return (T_REDIR_OUT);
	if (my_strcmp(s, ">>") == 0)
		return (REDIR_APPEND);
	if (my_strcmp(s, "<<") == 0)
		return (REDIR_HEREDOC);
	return (T_REDIR_IN);
}
