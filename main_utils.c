/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:56:51 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/24 12:42:29 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_in_out_fds(t_in_out_fds *redir)
{
	t_in_out_fds	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->filename)
		{
			unlink(tmp->filename);
			free(tmp->filename);
		}
		free(tmp);
	}
}

void	free_all(t_cmd *cmds, t_env *env)
{
	if (cmds)
		free_cmds(&cmds);
	if (env)
		env_free_all(env);
	rl_clear_history();
}

void	handle_exit(t_cmd *cmds, t_env *env)
{
	int	ret;

	ret = ft_exit(cmds->args, &g_exit);
	if (ret == -1)
	{
		free_all(cmds, env);
		exit(g_exit);
	}
}

void	check_if_cmd(t_cmd *cmd, t_env **env)
{
	int	run;
	int	skip_cmd;

	skip_cmd = 0;
	if (cmd)
	{
		if (handle_all_heredocs(cmd, *env) != 0)
			skip_cmd = 1;
		if (!skip_cmd && cmd->args && cmd->args[0])
		{
			run = (!cmd->next && !cmd->io_fds);
			if (ft_strcmp(cmd->args[0], "exit") == 0)
				handle_exit(cmd, *env);
			else if (is_builtin(cmd->args[0]) && run)
				run_builtin(cmd->args, env, &g_exit);
			else if (cmd->next || cmd->io_fds)
				g_exit = pipeline_run(cmd, *env);
			else
				exec_cmd(cmd, *env, &g_exit);
		
		}
	}
}
