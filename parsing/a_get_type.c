/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_get_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:24:13 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/31 12:24:17 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token_type	get_type(char *s)
{
	if (my_strcmp(s, "|") == 0)
		return (free(s), PIPE);
	if (my_strcmp(s, "<") == 0)
		return (free(s), REDIR_IN);
	if (my_strcmp(s, ">") == 0)
		return (free(s), REDIR_OUT);
	if (my_strcmp(s, ">>") == 0)
		return (free(s), APPEND);
	if (my_strcmp(s, "<<") == 0)
		return (free(s), HEREDOC);
	return (free(s), WORD);
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
