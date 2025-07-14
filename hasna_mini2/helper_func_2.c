/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:04:44 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/14 02:23:37 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_cmd	*parse_commands(char **tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*temp_args;
	int		i;
	t_cmd	*tmp;

	head = NULL;
	current = new_cmd();
	temp_args = NULL;
	i = 0;
	while (tokens[i])
	{
		if (strcmp(tokens[i], "|") == 0)
		{
			current->args = token_list_to_array(temp_args);
			current->pipe_output = true;
			if (!head)
				head = current;
			else
			{
				tmp = head;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = current;
			}
			current = new_cmd();
			free_token_list(temp_args);
			temp_args = NULL;
		}
		else if (is_redirection(tokens[i]))
		{
			if (tokens[i + 1])
			{
				add_redirection(current, get_redirect_type(tokens[i]), tokens[i
					+ 1]);
				i++;
			}
		}
		else
		{
			add_token(&temp_args, tokens[i], WORD);
		}
		i++;
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
	i = 0;
	while (cmds)
	{
		printf("=== Command %d ===\n", num++);
		if (cmds->args)
		{
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
