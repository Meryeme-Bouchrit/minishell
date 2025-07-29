/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 02:08:19 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/27 09:32:58 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_path(char **envp)
{
	int	i = 0;

	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*find_bin(char *cmd, char **paths)
{
	int		i = 0;
	char	*tmp;
	char	*full;

	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}


void	redir_in(char *filename)
{
	int	fd = open(filename, O_RDONLY);
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

void	redir_out(char *filename)
{
	int	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

void	redir_app(char *filename)
{
	int	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

void	redir_heredoc(char *limiter)
{
	int		pipe_fd[2];
	char	*line;

	pipe(pipe_fd);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0)
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
