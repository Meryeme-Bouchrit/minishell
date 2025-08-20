/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:19:20 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/20 00:22:26 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

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
	token->expand = true;
	token->next = NULL;
	return (token);
}

void	split_token(t_token **head, char *val, t_token *new, t_token *temp)
{
	char	**arg;
	char	**arg_tmp;

	arg = NULL;
	arg = ft_split(val, ' ');
	arg_tmp = arg;
	while (arg && *arg)
	{
		if (!*head)
		{
			new = new_token(ft_strdup(*arg), WORD);
			*head = new;
		}
		else
		{
			temp = *head;
			while (temp->next)
				temp = temp->next;
			temp->next = new_token(ft_strdup(*arg), WORD);
		}
		arg++;
	}
	free_split(arg_tmp);
	free(val);
}

void	add_token(t_token **head, char *val, t_token_type type)
{
	t_token	*new;
	t_token	*temp;

	new = NULL;
	temp = NULL;
	if (type == SPLIT)
		split_token(head, val, new, temp);
	else
	{
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
}
