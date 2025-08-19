/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 02:11:00 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/19 14:48:19 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	close_all_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

int	count_cmds(t_cmd *cmds)
{
	int		n;
	t_cmd	*cur;

	n = 0;
	cur = cmds;
	while (cur)
	{
		n++;
		cur = cur->next;
	}
	return (n);
}

int	create_all_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

void	setup_child(int **pipes, int idx, int total)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (idx > 0)
		dup2(pipes[idx - 1][0], 0);
	if (idx < total - 1)
		dup2(pipes[idx][1], 1);
	close_all_pipes(pipes, total - 1);
}

void	print_exit(char *cmd, char *msg, int code)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, msg, ft_strlen(msg));
	exit(code);
}

void	check_cmd_errors(t_cmd *cmd)
{
	int	fd;

	if (!cmd->args[0] || !cmd->args[0][0])
		exit(0);
	if (ft_strchr(cmd->args[0], '/'))
	{
		fd = open(cmd->args[0], O_DIRECTORY);
		if (fd != -1)
		{
			close(fd);
			print_exit(cmd->args[0], ": Is a directory\n", 126);
		}
		if (access(cmd->args[0], F_OK) == 0 && access(cmd->args[0], X_OK) != 0)
			print_exit(cmd->args[0], ": Permission denied\n", 126);
		if (access(cmd->args[0], F_OK) != 0)
			print_exit(cmd->args[0], ": No such file or directory\n", 127);
	}
}

void	run_cmd(t_cmd *cmd, t_env *env)
{
	char	**envp;
	char	*path;

	envp = env_to_envp(env);
	path = find_cmd_path(cmd->args[0], envp);
	if (!path)
	{
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, ": command not found\n", 20);
		free_split(envp);
		exit(127);
	}
	execve(path, cmd->args, envp);
	perror(cmd->args[0]);
	free(path);
	free_split(envp);
	exit(127);
}

void	child_process(t_cmd *cmd, t_env *env)
{
	int	exit_code;

	exit_code = 0;
	setup_child(cmd->pipe_fds, cmd->idx, cmd->total);
	apply_all_redirs(cmd->io_fds);
	if (is_builtin(cmd->args[0]))
	{
		run_builtin(cmd->args, &env, &exit_code);
		exit(exit_code);
	}
	check_cmd_errors(cmd);
	run_cmd(cmd, env);
}

int	**init_pipes(int n)
{
	int	**pipes;

	if (n <= 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * (n - 1));
	if (!pipes || create_all_pipes(pipes, n - 1))
	{
		if (pipes)
			free(pipes);
		return (NULL);
	}
	return (pipes);
}

pid_t	*init_pids(int n, int **pipes)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
	{
		if (pipes)
			close_all_pipes(pipes, n - 1);
		return (NULL);
	}
	return (pids);
}

int	fork_one(t_cmd *cmd, t_env *env, pid_t *pids, int i)
{
	pids[i] = fork();
	if (pids[i] == -1)
	{
		perror("fork");
		return (1);
	}
	if (pids[i] == 0)
		child_process(cmd, env);
	return (0);
}

int	wait_last(pid_t *pids, int n)
{
	int	i;
	int	status;

	i = 0;
	while (i < n)
	{
		if (pids[i] > 0 && waitpid(pids[i], &status, 0) > 0 && i == n - 1)
		{
			if (status >= 0 && status <= 255)
				return (status);
			if ((status % 256) == SIGINT)
			{
				write(1, "\n", 1);
				return (130);
			}
			if ((status % 256) == SIGQUIT)
			{
				write(2, "Quit (core dumped)\n", 19);
				return (131);
			}
			return (128 + (status % 256));
		}
		i++;
	}
	return (0);
}

int	fork_all(t_cmd *cmds, t_env *env, pid_t *pids)
{
	t_cmd	*cur;
	int		i;
	int		ret;

	cur = cmds;
	i = 0;
	ret = 0;
	while (cur)
	{
		cur->idx = i;
		if (fork_one(cur, env, pids, i))
		{
			ret = 1;
			break ;
		}
		cur = cur->next;
		i++;
	}
	return (ret);
}

int	end_pipeline(pid_t *pids, int **pipes, int n, int ret)
{
	if (pipes)
		close_all_pipes(pipes, n - 1);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (!ret)
		ret = wait_last(pids, n);
	signal(SIGINT, sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
	free(pids);
	return (ret);
}

int	exec_pipeline(t_cmd *cmds, t_env *env)
{
	int		n;
	int		**pipes;
	pid_t	*pids;
	int		ret;
	t_cmd	*cur;

	ret = 0;
	signal(SIGINT, sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
	n = count_cmds(cmds);
	if (n == 0)
		return (0);
	pipes = init_pipes(n);
	pids = init_pids(n, pipes);
	if (!pids)
		return (1);
	cur = cmds;
	while (cur)
	{
		cur->total = n;
		cur->pipe_fds = pipes;
		cur = cur->next;
	}
	ret = fork_all(cmds, env, pids);
	return (end_pipeline(pids, pipes, n, ret));
}
