/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_func_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:04:18 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/14 07:04:22 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	token_list_size(t_token *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

char	**token_list_to_array(t_token *list)
{
	int		size;
	char	**array;
	int		i;

	size = token_list_size(list);
	array = malloc(sizeof(char *) * (size + 1));
	i = 0;
	while (list)
	{
		array[i++] = ft_strdup(list->value);
		list = list->next;
	}
	array[i] = NULL;
	return (array);
}

void	free_token_list(t_token *list)
{
	t_token	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->value);
		free(tmp);
	}
}

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	cmd->args = NULL;
	cmd->io_fds = NULL;
	cmd->pipe_output = false;
	cmd->next = NULL;
	return (cmd);
}

void	add_redirection(t_cmd *cmd, t_redirections type, const char *filename)
{
	t_in_out_fds	*redir;
	t_in_out_fds	*tmp;

	redir = malloc(sizeof(t_in_out_fds));
	redir->type = type;
	redir->filename = ft_strdup(filename);
	redir->next = NULL;
	if (!cmd->io_fds)
		cmd->io_fds = redir;
	else
	{
		tmp = cmd->io_fds;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
}
