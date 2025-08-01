/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:47:33 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/27 10:02:09 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	only_spaces(const char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	run_builtin(char **args, t_env **env, int *exit_status)
{
	if (ft_strcmp(args[0], "echo") == 0)
		*exit_status = ft_echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		*exit_status = ft_cd(args, env);
	else if (ft_strcmp(args[0], "pwd") == 0)
		*exit_status = ft_pwd();
	else if (ft_strcmp(args[0], "export") == 0)
		*exit_status = ft_export(args, env);
	else if (ft_strcmp(args[0], "unset") == 0)
		*exit_status = ft_unset(args, env);
	else if (ft_strcmp(args[0], "env") == 0)
		*exit_status = ft_env(*env);
	else if (ft_strcmp(args[0], "exit") == 0)
		ft_exit(args, exit_status);
	else
		return (0);
	return (1);
}

int	exec_builtin_with_redir(t_cmd *cmd, t_env **env, int *exit_status)
{
	int				in_backup;
	int				out_backup;
	t_in_out_fds	*redir;
	int				result;

	in_backup = dup(STDIN_FILENO);
	out_backup = dup(STDOUT_FILENO);
	redir = cmd->io_fds;
	while (redir)
	{
		if (redir->type == T_REDIR_IN)
			redir_in(redir->filename);
		else if (redir->type == T_REDIR_OUT)
			redir_out(redir->filename);
		else if (redir->type == REDIR_APPEND)
			redir_app(redir->filename);
		else if (redir->type == REDIR_HEREDOC)
			redir_heredoc(redir->filename);
		redir = redir->next;
	}
	result = run_builtin(cmd->args, env, exit_status);
	dup2(in_backup, STDIN_FILENO);
	dup2(out_backup, STDOUT_FILENO);
	close(in_backup);
	close(out_backup);
	return (result);
}
