/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:55:20 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 19:08:51 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	redir_file(char *file, int target_fd, int flags)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		write(2, "minishell: ", 12);
		perror(file);
		return (1);
	}
	if (dup2(fd, target_fd) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

void	apply_redir(t_in_out_fds *redir)
{
	int	fd;

	if (redir->type == REDIR_HEREDOC)
	{
		fd = open(redir->filename, O_RDONLY);
		if (fd < 0 || dup2(fd, 0) == -1)
		{
			perror("heredoc");
			close(fd);
			exit(1);
		}
		close(fd);
	}
	else if (redir->type == T_REDIR_IN && redir_file(redir->filename, 0,
			O_RDONLY))
		exit(1);
	else if (redir->type == T_REDIR_OUT && redir_file(redir->filename, 1,
			O_WRONLY | O_CREAT | O_TRUNC))
		exit(1);
	else if (redir->type == REDIR_APPEND && redir_file(redir->filename, 1,
			O_WRONLY | O_CREAT | O_APPEND))
		exit(1);
}

void	apply_all_redirs(t_in_out_fds *redir)
{
	while (redir)
	{
		apply_redir(redir);
		redir = redir->next;
	}
}

int	write_heredoc(int fd, char *line, t_env *env, bool expand)
{
	char	*tmp;

	if (expand)
	{
		tmp = expand_variables(line, env);
		if (tmp)
		{
			ft_putstr_fd(tmp, fd);
			ft_putstr_fd("\n", fd);
			free(tmp);
		}
	}
	else
	{
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
	}
	return (0);
}

int	heredoc_input(int fd, char *limiter, t_env *env, bool expand)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc(fd, line, env, expand);
		free(line);
	}
	return (0);
}
