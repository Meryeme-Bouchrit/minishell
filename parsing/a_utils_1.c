/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_utils_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 08:53:30 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/27 10:12:59 by mbouchri         ###   ########.fr       */
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
	token->freed = false;
	token->next = NULL;
	return (token);
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

/*
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
*/
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
		printf("Error: quote not closed\n");
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
