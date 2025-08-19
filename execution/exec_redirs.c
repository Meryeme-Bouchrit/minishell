/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:49:35 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/19 15:27:43 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	redirect_file(char *file, int target_fd, int flags)
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
	else if (redir->type == T_REDIR_IN && redirect_file(redir->filename, 0,
			O_RDONLY))
		exit(1);
	else if (redir->type == T_REDIR_OUT && redirect_file(redir->filename, 1,
			O_WRONLY | O_CREAT | O_TRUNC))
		exit(1);
	else if (redir->type == REDIR_APPEND && redirect_file(redir->filename, 1,
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

// char *read_line(void)
// {
//     char *line = NULL;
//     size_t len = 0;
//     ssize_t n;

//     n = getline(&line, &len, stdin);
//     if (n <= 0)
//     {
//         free(line);
//         return NULL; // EOF or error
//     }
//     if (line[n - 1] == '\n')
//         line[n - 1] = '\0';
//     return line;
// }



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

char	*new_heredoc_name(void)
{
	static int	counter = 0;
	char		*num;
	char		*file;

	num = ft_itoa(counter++);
	file = ft_strjoin("/tmp/ms_heredoc_", num);
	free(num);
	return (file);
}

int	create_heredoc_fd(char **path)
{
	int	fd;

	*path = new_heredoc_name();
	if (!*path)
		return (-1);
	while (1)
	{
		fd = open(*path, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd != -1)
			break ;
		free(*path);
		*path = new_heredoc_name();
		if (!*path)
			return (-1);
	}
	return (fd);
}

int	fork_heredoc(int fd, char *limiter, t_env *env, bool expand)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, sigint_heredoc);
		signal(SIGQUIT, SIG_IGN);
		heredoc_input(fd, limiter, env, expand);
		close(fd);
		exit(0);
	}
	close(fd);
	return (pid);
}

char	*wait_heredoc(pid_t pid, char *path)
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

char	*make_heredoc(char *limiter, t_env *env, bool expand)
{
	char	*path;
	int		fd;
	int		pid;

	fd = create_heredoc_fd(&path);
	if (fd == -1)
		return (NULL);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork_heredoc(fd, limiter, env, expand);
	if (pid == -1)
	{
		unlink(path);
		free(path);
		signal(SIGINT, sigint_prompt);
		signal(SIGQUIT, SIG_IGN);
		return (NULL);
	}
	return (wait_heredoc(pid, path));
}

void	remove_cmd_heredocs(t_cmd *cmd)
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
		remove_cmd_heredocs(cmd);
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

void	cleanup_all_heredocs(t_cmd *cmds)
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
