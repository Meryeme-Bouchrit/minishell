/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:11:28 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/24 18:25:23 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	handle_pipe(t_cmd **head, t_cmd **current, t_token **temp_args)
{
	t_cmd	*tmp;

	(*current)->args = token_list_to_array(*temp_args);
	(*current)->pipe_output = true;
	if (!*head)
		*head = *current;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = *current;
	}
	*current = new_cmd();
	free_token_list(*temp_args);
	*temp_args = NULL;
}

int	parsing_redir(t_token **tokens, t_cmd **head, t_cmd **current,
		t_token **temp_args)
{
	if ((*tokens)->next->value && (*tokens)->next->type == WORD)
	{
		add_redirection(*current, get_redirect_type((*tokens)->value),
			(*tokens)->next->value, (*tokens)->expand);
		*tokens = (*tokens)->next;
	}
	else
	{
		free_cmds(current);
		free_cmds(head);
		g_exit = 2;
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		if (*temp_args)
			free_token_list(*temp_args);
		return (1);
	}
	return (0);
}

void	last_parsing(t_cmd **tmp, t_cmd **head, t_cmd **current,
		t_token **temp_args)
{
	if (*temp_args)
	{
		(*current)->args = token_list_to_array(*temp_args);
		free_token_list(*temp_args);
	}
	if (!(*head))
		*head = *current;
	else
	{
		*tmp = *head;
		while ((*tmp)->next)
			*tmp = (*tmp)->next;
		(*tmp)->next = *current;
	}
}

t_cmd	*parse_commands(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*temp_args;
	t_cmd	*tmp;

	head = NULL;
	tmp = NULL;
	current = new_cmd();
	temp_args = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
			handle_pipe(&head, &current, &temp_args);
		else if (redirection(tokens->type))
		{
			if (parsing_redir(&tokens, &head, &current, &temp_args))
				return (NULL);
		}
		else
			add_token(&temp_args, ft_strdup(tokens->value), tokens->type);
		tokens = tokens->next;
	}
	last_parsing(&tmp, &head, &current, &temp_args);
	return (head);
}
