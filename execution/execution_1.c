/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:52:12 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/27 11:28:31 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_execute_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	cmd_path = ft_get_command_path(cmd->args[0], envp);
	if (!cmd_path)
	{
		perror("cmd");
		return (127);
	}
	pid = fork();
	if (pid == 0)
		ft_child_process(cmd, cmd_path, envp);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
	}
	else
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}
	return (0);
}

char	*ft_get_command_path(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*full;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path = get_path(envp);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	full = find_bin(cmd, paths);
	free_split(paths);
	return (full);
}

void	ft_child_process(t_cmd *cmd, char *cmd_path, char **envp)
{
	t_in_out_fds	*redir;

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
	execve(cmd_path, cmd->args, envp);
	perror("execve");
	exit(127);
}
