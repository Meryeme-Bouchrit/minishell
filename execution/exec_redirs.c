/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:22:10 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/19 14:30:00 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/* -------------------- SIMPLE REDIRECTION -------------------- */

int	redirect_fd(char *file, int fd_target, int flags)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		write(2, "minishell: ", 12);
		perror(file);
		return (1);
	}
	if (dup2(fd, fd_target) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

void	handle_single_redir(t_in_out_fds *redir)
{
	if (redir->type == REDIR_HEREDOC)
	{
		int	fd = open(redir->filename, O_RDONLY);
		if (fd < 0 || dup2(fd, 0) == -1)
		{
			perror("heredoc");
			close(fd);
			exit(1);
		}
		close(fd);
	}
	else if (redir->type == T_REDIR_IN && redirect_fd(redir->filename, 0, O_RDONLY))
		exit(1);
	else if (redir->type == T_REDIR_OUT
		&& redirect_fd(redir->filename, 1, O_WRONLY | O_CREAT | O_TRUNC))
		exit(1);
	else if (redir->type == REDIR_APPEND
		&& redirect_fd(redir->filename, 1, O_WRONLY | O_CREAT | O_APPEND))
		exit(1);
}

void	handle_redirections(t_in_out_fds *redir)
{
	while (redir)
	{
		handle_single_redir(redir);
		redir = redir->next;
	}
}

/* -------------------- HEREDOC HELPERS -------------------- */

int	write_heredoc_line(int fd, char *line, t_env *env, bool expand)
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

int	heredoc_loop(int fd, char *limiter, t_env *env, bool expand)
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
		write_heredoc_line(fd, line, env, expand);
		free(line);
	}
	return (0);
}

char	*generate_heredoc_filename(void)
{
	static int	counter = 0;
	char		*num;
	char		*file;

	num = ft_itoa(counter++);
	if (!num)
		return (NULL);
	file = ft_strjoin("/tmp/ms_heredoc_", num);
	free(num);
	return (file);
}

int	create_heredoc_file(char **path)
{
	int	fd;

	*path = generate_heredoc_filename();
	if (!*path)
		return (-1);
	while (1)
	{
		fd = open(*path, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd != -1)
			break ;
		free(*path);
		*path = generate_heredoc_filename();
		if (!*path)
			return (-1);
	}
	return (fd);
}

int	fork_heredoc_child(int fd, char *limiter, t_env *env, bool expand)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, sigint_heredoc);
		signal(SIGQUIT, SIG_IGN);
		heredoc_loop(fd, limiter, env, expand);
		close(fd);
		exit(0);
	}
	close(fd);
	return (pid);
}

char	*wait_heredoc_child(char *path, pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1 || status != 0)
	{
		unlink(path);
		free(path);
		return (NULL);
	}
	return (path);
}

char	*create_heredoc(char *limiter, t_env *env, bool expand)
{
	char	*path;
	int		fd;
	int		pid;

	fd = create_heredoc_file(&path);
	if (fd == -1)
		return (NULL);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork_heredoc_child(fd, limiter, env, expand);
	if (pid == -1)
	{
		unlink(path);
		free(path);
		signal(SIGINT, sigint_prompt);
		signal(SIGQUIT, SIG_IGN);
		return (NULL);
	}
	return (wait_heredoc_child(path, pid));
}

/* -------------------- HEREDOC CLEANUP -------------------- */

void	cleanup_cmd_heredoc(t_cmd *cmd)
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

int	process_heredocs(t_cmd *cmd, t_env *env)
{
	t_in_out_fds	*redir;
	char			*file;

	redir = cmd->io_fds;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			file = create_heredoc(redir->filename, env, redir->expand);
			if (!file)
				return (1);
			free(redir->filename);
			redir->filename = file;
		}
		redir = redir->next;
	}
	if (!cmd->args || !cmd->args[0])
		cleanup_cmd_heredoc(cmd);
	return (0);
}

int	preprocess_heredocs(t_cmd *cmds, t_env *env)
{
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (process_heredocs(cmd, env))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

void	cleanup_all_heredocs(t_cmd *cmds)
{
	t_cmd			*cmd;
	t_in_out_fds	*redir;

	cmd = cmds;
	while (cmd)
	{
		redir = cmd->io_fds;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC && redir->filename)
				unlink(redir->filename);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}
