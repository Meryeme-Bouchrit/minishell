/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:52:12 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/15 16:24:45 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	cmd_path = get_command_path(cmd->args[0], envp);
	if (!cmd_path)
	{
		perror("cmd");
		return (127);
	}
	pid = fork();
	if (pid == 0)
		child_process(cmd, cmd_path, envp);
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


char	*get_command_path(char *cmd, char **envp)
{
	int		i = 0;
	int		j;
	char	*path_value = NULL;
	char	**paths;
	char	*full_path;
	char	*tmp;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_value = envp[i] + 5;
			break;
		}
		i++;
	}
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	j = 0;
	while (paths[j])
	{
		tmp = ft_strjoin(paths[j], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		j++;
	}
	free_split(paths);
	return (NULL);
}

void	child_process(t_cmd *cmd, char *cmd_path, char **envp)
{
	t_in_out_fds *redir;
	int fd;
	int pipe_fd[2];
	char *line;

	redir = cmd->io_fds;
	while (redir)
	{
		if (redir->type == T_REDIR_IN)
		{
			fd = open(redir->filename, O_RDONLY);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			if (dup2(fd, 0) == -1)
			{
				perror("dup2");
				close(fd);
				exit(1);
			}
			close(fd);
		}
		else if (redir->type == T_REDIR_OUT)
		{
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			if (dup2(fd, 1) == -1)
			{
				perror("dup2");
				close(fd);
				exit(1);
			}
			close(fd);
		}
		else if (redir->type == REDIR_APPEND)
		{
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			if (dup2(fd, 1) == -1)
			{
				perror("dup2");
				close(fd);
				exit(1);
			}
			close(fd);
		}
		else if (redir->type == REDIR_HEREDOC)
		{
			pipe(pipe_fd);
			while (1)
			{
				line = readline("> ");
				if (!line || ft_strcmp(line, redir->filename) == 0)
					break;
				write(pipe_fd[1], line, ft_strlen(line));
				write(pipe_fd[1], "\n", 1);
				free(line);
			}
			free(line);
			close(pipe_fd[1]);
			if (dup2(pipe_fd[0], 0) == -1)
			{
				perror("dup2");
				close(pipe_fd[0]);
				exit(1);
			}
			close(pipe_fd[0]);
		}
		redir = redir->next;
	}
	execve(cmd_path, cmd->args, envp);
	perror("execve");
	exit(127);
}
