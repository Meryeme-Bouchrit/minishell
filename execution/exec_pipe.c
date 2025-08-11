/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:22:44 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/11 04:03:58 by mbouchri         ###   ########.fr       */
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
		i++;
	}
}

int	create_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipe(pipes[i]) == -1)
			return (ft_putstr_fd("minishell: pipe error\n", 2), 1);
		i++;
	}
	return (0);
}

void	child_process_pipeline(t_cmd *cmd, t_env *env, int **pipes, int i, int total)
{
	if (i > 0)
		dup2(pipes[i - 1][0], 0);
	if (i < total - 1)
		dup2(pipes[i][1], 1);
	close_pipes(pipes, total - 1);
	ft_handle_redirs(cmd->io_fds, env);
	if (is_builtin(cmd->args[0]))
	{
		int	exit_code = 0;
		run_builtin(cmd->args, &env, &exit_code);
		exit(exit_code);
	}
	else
		exec_cmd(cmd, env);
}

int	exec_pipeline(t_cmd *cmds, t_env *env)
{
	int		n;
	int		**pipes;
	pid_t	*pids;
	t_cmd	*cur;
	int		i;
	int		status;

	n = 0;
	cur = cmds;
	while (cur && ++n)
		cur = cur->next;
	pipes = malloc(sizeof(int *) * (n - 1));
	if (!pipes)
		return (1);
	i = -1;
	while (++i < n - 1)
		pipes[i] = malloc(sizeof(int) * 2);
	if (create_pipes(pipes, n - 1))
		return (1);
	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		return (1);
	cur = cmds;
	i = -1;
	while (++i < n)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			child_process_pipeline(cur, env, pipes, i, n);
		cur = cur->next;
	}
	close_pipes(pipes, n - 1);
	i = -1;
	while (++i < n)
		waitpid(pids[i], &status, 0);
	free(pids);
	while (--n > 0)
		free(pipes[n - 1]);
	free(pipes);
	return (0);
}
