/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_manage.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:59:31 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/22 10:59:46 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	remove_cmd_heredocs(t_cmd *cmd)
{
	t_in_out_fds	*redir;

	redir = cmd->io_fds;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC && redir->filename)
		{
			unlink(redir->filename);
			free(redir->filename);
			redir->filename = NULL;
		}
		redir = redir->next;
	}
}

int	handle_heredocs(t_cmd *cmd, t_env *env)
{
	t_in_out_fds	*redir;
	char			*file;

	redir = cmd->io_fds;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			file = make_heredoc(redir->filename, env, redir->expand);
			if (!file)
				return (1);
			free(redir->filename);
			redir->filename = file;
		}
		redir = redir->next;
	}
	if (!cmd->args || !cmd->args[0])
		remove_cmd_heredocs(cmd);
	return (0);
}

int	handle_all_heredocs(t_cmd *cmds, t_env *env)
{
	while (cmds)
	{
		if (handle_heredocs(cmds, env))
			return (1);
		cmds = cmds->next;
	}
	return (0);
}

void	cleanup_all_heredocs(t_cmd *cmds)
{
	t_in_out_fds	*redir;

	while (cmds)
	{
		redir = cmds->io_fds;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC && redir->filename)
				unlink(redir->filename);
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}
