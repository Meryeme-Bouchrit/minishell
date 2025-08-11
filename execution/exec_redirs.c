/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:22:10 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/11 04:02:26 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (perror(file), -1);
	if (dup2(fd, 0) == -1)
		return (perror("dup2"), close(fd), -1);
	close(fd);
	return (0);
}

int	redir_out(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror(file), -1);
	if (dup2(fd, 1) == -1)
		return (perror("dup2"), close(fd), -1);
	close(fd);
	return (0);
}

int	redir_app(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (perror(file), -1);
	if (dup2(fd, 1) == -1)
		return (perror("dup2"), close(fd), -1);
	close(fd);
	return (0);
}

int	write_heredoc(int fd, char *limiter, t_env *env, bool expand)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0)
			return (free(line), 0);
		if (expand)
		{
			expanded = expand_variables(line, env);
			free(line);
			if (!expanded)
				return (-1);
			line = expanded;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

int	redir_heredoc(char *limiter, t_env *env, bool expand)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (-1);
	if (write_heredoc(pipe_fd[1], limiter, env, expand) == -1)
		return (close(pipe_fd[0]), close(pipe_fd[1]), -1);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

void	ft_handle_redirs(t_in_out_fds *r, t_env *env)
{
	int	fd;

	while (r)
	{
		if (r->type == T_REDIR_IN)
			redir_in(r->filename);
		else if (r->type == T_REDIR_OUT)
			redir_out(r->filename);
		else if (r->type == REDIR_APPEND)
			redir_app(r->filename);
		else if (r->type == REDIR_HEREDOC)
		{
			fd = redir_heredoc(r->filename, env, r->expand);
			if (dup2(fd, 0) == -1)
				return (perror("dup2"), close(fd), exit(1));
			close(fd);
		}
		r = r->next;
	}
}
