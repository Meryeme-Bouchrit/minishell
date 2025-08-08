/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:00:31 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/08 23:10:09 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_execute_cmd(t_cmd *cmd, t_env *env)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**envp_arr;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);

	envp_arr = env_to_envp(env);
	cmd_path = ft_get_command_path_envp(cmd->args[0], envp_arr);
	free_split(envp_arr);

	if (!cmd_path)
	{
		perror("cmd");
		return (127);
	}

	pid = fork();
	if (pid == 0)
		ft_child_process(cmd, cmd_path, env);
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

char	*ft_get_command_path_envp(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*full;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path = get_path_envp(envp);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	full = find_bin(cmd, paths);
	free_split(paths);
	return (full);
}

char	*get_path_envp(char **envp)
{
	int i = 0;

	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

void	ft_child_process(t_cmd *cmd, char *cmd_path, t_env *env)
{
	t_in_out_fds	*redir;
	int				heredoc_fd;
	char			**envp_arr;

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
		{
			heredoc_fd = redir_heredoc(redir->filename, env, redir->expand);
			if (dup2(heredoc_fd, 0) == -1)
			{
				perror("dup2");
				close(heredoc_fd);
				exit(1);
			}
			close(heredoc_fd);
		}
		redir = redir->next;
	}
	envp_arr = env_to_envp(env);
	execve(cmd_path, cmd->args, envp_arr);
	perror("execve");
	free_split(envp_arr);
	exit(127);
}

