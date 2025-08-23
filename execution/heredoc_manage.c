/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_manage.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:59:31 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 19:53:27 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
		heredocs_cleanup(cmd);
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

void	heredocs_cleanup(t_cmd *cmds)
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
