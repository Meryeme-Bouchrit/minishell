/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:37:15 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/14 09:27:28 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

/*
char	*extract_quoted(const char *str, int *i, char quote)
{
	int		start;
	int		len;
	char	*result;

	start = *i;
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	len = *i - start - 1;
	result = malloc(len + 1);
	ft_strncpy(result, str + start + 1, len);
	result[len] = '\0';
	if (str[*i] == quote)
		(*i)++;
	return (result);
}
*/

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
