/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:39:28 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/22 12:16:36 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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

int	wait_last(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
			{
				write(1, "\n", 1);
				last_status = 130;
			}
			else if (WTERMSIG(status) == SIGQUIT)
			{
				write(2, "Quit\n", 5);
				last_status = 131;
			}
		}
		else if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else
			last_status = 1;
		i++;
	}
	return (last_status);
}
