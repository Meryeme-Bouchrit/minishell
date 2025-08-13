/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_to_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:04:44 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/08 15:59:40 by zhassna          ###   ########.fr       */
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

t_cmd	*parse_commands(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*temp_args;
	t_cmd	*tmp;

	head = NULL;
	current = new_cmd();
	temp_args = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
			handle_pipe(&head, &current, &temp_args);
		else if (redirection(tokens->type))
		{
			if (tokens->next->value && tokens->next->type == WORD)
			{
				add_redirection(current, get_redirect_type(tokens->value),
					tokens->next->value, tokens->expand);
				tokens = tokens->next;
			}
			else
			{
				free_cmds(&current);
				free_cmds(&head);
				write(2, "ERROR! syntax error near unexpected token `|'\n", 47);
				if (temp_args)
					free_token_list(temp_args);
				return (NULL);
			}
		}
		else
		{
			add_token(&temp_args, ft_strdup(tokens->value), tokens->type);
		}
		tokens = tokens->next;
	}
	if (temp_args)
	{
		current->args = token_list_to_array(temp_args);
		free_token_list(temp_args);
	}
	if (!head)
		head = current;
	else
	{
		tmp = head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = current;
	}
	return (head);
}

void	print_commands(t_cmd *cmds)
{
	int				num;
	int				i;
	t_in_out_fds	*redir;

	num = 1;
	while (cmds)
	{
		printf("=== Command %d ===\n", num++);
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
			{
				printf("Arg[%d]: %s\n", i, cmds->args[i]);
				i++;
			}
		}
		if (cmds->io_fds)
		{
			redir = cmds->io_fds;
			while (redir)
			{
				printf("Redirection: type %d, file %s\n", redir->type,
					redir->filename);
				redir = redir->next;
			}
		}
		if (cmds->pipe_output)
			printf("Pipe output? Yes\n\n");
		else
			printf("Pipe output? No\n\n");
		cmds = cmds->next;
	}
}
