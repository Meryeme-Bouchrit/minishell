/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:23:58 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/14 07:04:07 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

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

char	*ft_substr(const char *s, int start, int len)
{
	int		i;
	char	*sub;

	i = 0;
	if (!s)
		return (NULL);
	sub = malloc(len + 1);
	if (!sub)
		return (NULL);
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[len] = '\0';
	return (sub);
}

void	add_token(t_token **head, char *val, t_token_type type)
{
	t_token	*new;
	t_token	*temp;

	new = new_token(val, type);
	if (!*head)
		*head = new;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}
