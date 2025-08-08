/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 02:08:19 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/08 23:15:17 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char *get_path(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char *find_bin(char *cmd, char **paths)
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

void redir_in(char *filename)
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

void redir_out(char *filename)
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

void redir_app(char *filename)
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

int redir_heredoc(char *limiter, t_env *env, bool expand)
{
    int     pipe_fd[2];
    char    *line;
    char    *expanded;

    if (pipe(pipe_fd) == -1)
        return (-1);
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
        if (expand)
        {
            expanded = expand_variables(line, env);
            if (!expanded)
            {
                free(line);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
                return (-1);
            }
            write(pipe_fd[1], expanded, ft_strlen(expanded));
            write(pipe_fd[1], "\n", 1);
            free(expanded);
        }
        else
        {
            write(pipe_fd[1], line, ft_strlen(line));
            write(pipe_fd[1], "\n", 1);
        }
        free(line);
    }
    close(pipe_fd[1]);
    return (pipe_fd[0]);
}

int ft_execute_pipeline(t_cmd *cmd, t_env *env)
{
	int				fd;
	int				pid;
	int				pipefd[2];
	int				prev = -1;
	t_in_out_fds	*redir;
	t_cmd			*curr;
	pid_t			*pid_arr;
	int				num_cmds = 0;
	int				i = 0;
	int				j = 0;

	curr = cmd;
	while (curr)
	{
		num_cmds++;
		curr = curr->next;
	}
	pid_arr = malloc(sizeof(pid_t) * num_cmds);
	if (!pid_arr)
	{
		perror("malloc");
		exit(1);
	}

	while (cmd)
	{
		if (cmd->next != NULL && pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0)
		{
			bool stdin_redirected = false;

			// First: Apply heredoc and input redirections (these redirect stdin)
			redir = cmd->io_fds;
			while (redir)
			{
				if (redir->type == REDIR_HEREDOC)
				{
					fd = redir_heredoc(redir->filename, env, redir->expand);
					if (fd == -1)
					{
						perror("redir_heredoc");
						exit(1);
					}
					if (dup2(fd, 0) == -1)
					{
						perror("dup2 heredoc");
						close(fd);
						exit(1);
					}
					close(fd);
					stdin_redirected = true;
				}
				else if (redir->type == T_REDIR_IN)
				{
					redir_in(redir->filename);
					stdin_redirected = true;
				}
				redir = redir->next;
			}

			// Then: If stdin not redirected, dup2 prev pipe read end to stdin
			if (prev != -1 && !stdin_redirected)
			{
				if (dup2(prev, 0) == -1)
				{
					perror("dup2 prev pipe");
					exit(1);
				}
				close(prev);
			}

			// Apply output redirections (> and >>)
			redir = cmd->io_fds;
			while (redir)
			{
				if (redir->type == T_REDIR_OUT)
					redir_out(redir->filename);
				else if (redir->type == REDIR_APPEND)
					redir_app(redir->filename);
				redir = redir->next;
			}

			// If there is a next command, dup2 pipe write end to stdout
			if (cmd->next != NULL)
			{
				if (dup2(pipefd[1], 1) == -1)
				{
					perror("dup2 pipe write");
					exit(1);
				}
				close(pipefd[1]);
				close(pipefd[0]);
			}

			char **envp_arr = env_to_envp(env);
			char *cmd_path = ft_get_command_path_envp(cmd->args[0], envp_arr);
			if (!cmd_path)
			{
				perror("cmd");
				free_split(envp_arr);
				exit(127);
			}
			execve(cmd_path, cmd->args, envp_arr);
			perror("execve");
			free(cmd_path);
			free_split(envp_arr);
			exit(127);
		}
		pid_arr[i++] = pid;
		if (prev != -1)
			close(prev);
		if (cmd->next != NULL)
		{
			prev = pipefd[0];
			close(pipefd[1]);
		}
		cmd = cmd->next;
	}
	while (j < i)
	{
		waitpid(pid_arr[j], NULL, 0);
		j++;
	}
	free(pid_arr);
	return (0);
}


char **env_to_envp(t_env *env)
{
	int		len = 0;
	char	**envp;
	char	*tmp;
	t_env	*curr = env;

	while (curr)
	{
		len++;
		curr = curr->next;
	}
	envp = malloc(sizeof(char *) * (len + 1));
	if (!envp)
		return (NULL);
	curr = env;
	len = 0;
	while (curr)
	{
		tmp = ft_strjoin(curr->key, "=");
		envp[len++] = ft_strjoin(tmp, curr->value);
		free(tmp);
		curr = curr->next;
	}
	envp[len] = NULL;
	return (envp);
}
