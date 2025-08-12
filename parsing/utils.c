/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 08:53:30 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/12 06:17:25 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_isspace(char c)
{
	return (c == ' ');
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
	token->expand = false;
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

char	*extract_quoted(const char *str, int *i, char quote)
{
	int		start;
	int		len;
	char	*result;

	start = *i;
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i] != quote)
	{
		write(2, "Error: quote not closed\n", 25);
		exit(1);
	}
	len = *i - start - 1;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, str + start + 1, len);
	result[len] = '\0';
	if (str[*i] == quote)
		(*i)++;
	return (result);
}
