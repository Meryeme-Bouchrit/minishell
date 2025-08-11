/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:24:13 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/11 02:58:18 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token_type	get_type(char *s, int a)
{
	if (a)
		return (SPLIT);
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
