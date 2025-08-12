/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:22:44 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/12 16:03:40 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	close_pipes(int **pipes, int n)
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

int	create_pipes(int **pipes, int n)
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

void	child_process_pipeline(t_cmd *cmd, t_env *env,
			int **pipes, int i, int total)
{
	int		exit_code;
	char	**envp_arr;
	char	*path;

	exit_code = 0;
	if (i > 0)
		dup2(pipes[i - 1][0], 0);
	if (i < total - 1)
		dup2(pipes[i][1], 1);
	close_pipes(pipes, total - 1);
	ft_handle_redirs(cmd->io_fds, env);
	if (is_builtin(cmd->args[0]))
	{
		run_builtin(cmd->args, &env, &exit_code);
		exit(exit_code);
	}
	envp_arr = env_to_envp(env);
	path = find_cmd_path(cmd->args[0], envp_arr);
	if (!path)
	{
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, ": command not found\n", 20);
		free_split(envp_arr);
		exit(127);
	}
	execve(path, cmd->args, envp_arr);
	perror(cmd->args[0]);
	free(path);
	free_split(envp_arr);
	exit(127);
}

int exec_pipeline(t_cmd *cmds, t_env *env)
{
    int n;
    int **pipes;
    pid_t *pids;
    t_cmd *cur;
    int i;
    int status;
    int last_exit_code;

    n = 0;
    cur = cmds;
    while (cur && ++n)
        cur = cur->next;
    if (n == 0)
        return (0);
    pipes = malloc(sizeof(int *) * (n - 1));
    if (!pipes || create_pipes(pipes, n - 1))
        return (free(pipes), 1);
    pids = malloc(sizeof(pid_t) * n);
    if (!pids)
        return (close_pipes(pipes, n - 1), 1);
    cur = cmds;
    i = -1;
    while (++i < n)
    {
        pids[i] = fork();
        if (pids[i] == -1)
            return (perror("fork"), close_pipes(pipes, n - 1), free(pids), 1);
        if (pids[i] == 0)
            child_process_pipeline(cur, env, pipes, i, n);
        cur = cur->next;
    }
    close_pipes(pipes, n - 1);
    i = -1;
    while (++i < n)
    {
        waitpid(pids[i], &status, 0);
        if (i == n - 1)
            last_exit_code = status >> 8;
    }
    free(pids);
    return (last_exit_code);
}
