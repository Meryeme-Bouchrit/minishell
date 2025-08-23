/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:05:16 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 19:48:25 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	child_run(t_cmd *cmd, t_env *env)
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
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (check_cmd_errors(cmd->args[0], &exit_code) != 0)
			exit(exit_code);
	}
	exec_run(cmd, env);
}

void	child_single_run(t_cmd *cmd, char *path, t_env *env)
{
	char	**envp_arr;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	apply_all_redirs(cmd->io_fds);
	envp_arr = env_to_envp(env);
	if (!envp_arr)
	{
		perror("env_to_envp");
		exit(1);
	}
	execve(path, cmd->args, envp_arr);
	perror(cmd->args[0]);
	free_split(envp_arr);
	exit(127);
}

void	exec_run(t_cmd *cmd, t_env *env)
{
	char	**envp;
	char	*path;

	envp = env_to_envp(env);
	path = find_cmd_path(cmd->args[0], envp);
	if (!path)
	{
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, ": command not found\n", 21);
		free_split(envp);
		exit(127);
	}
	execve(path, cmd->args, envp);
	perror(cmd->args[0]);
	free(path);
	free_split(envp);
	exit(127);
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
		child_run(cmd, env);
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
			while (--i >= 0)
				kill(pids[i], SIGKILL);
			break ;
		}
		cur = cur->next;
		i++;
	}
	return (ret);
}
