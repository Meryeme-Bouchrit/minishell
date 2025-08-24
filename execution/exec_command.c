/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:01:59 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/24 22:23:39 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	exec_cmd(t_cmd *cmd, t_env *env, int *status)
{
	int	err;

	if (!cmd->args[0] || cmd->args[0][0] == '\0')
	{
		*status = 0;
		return (0);
	}
	if (is_builtin(cmd->args[0]))
	{
		*status = run_builtin(cmd->args, &env, status);
		return (*status);
	}
	if (ft_strchr(cmd->args[0], '/'))
	{
		err = check_cmd_errors(cmd->args[0], status);
		if (err != 0)
			return (err);
	}
	err = exec_resolve(cmd, env, status);
	if (err != 0)
		return (err);
	return (0);
}

int fork_and_wait(t_cmd *cmd, char *path, t_env *env)
{
    pid_t pid;
    int   status;
    int   ret;

    pid = fork();
    if (pid == -1)
        return (perror("fork"), 1);
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        child_single_run(cmd, path, env);
        return (0);
    }
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    waitpid(pid, &status, 0);
    signal(SIGINT, sigint_prompt);
    signal(SIGQUIT, SIG_IGN);
    ret = status_to_exit(status);
    if (ret == 130)
        write(1, "\n", 1);
    else if (ret == 131)
        write(2, "Quit (core dumped)\n", 20);
    return (ret);
}

int	exec_resolve(t_cmd *cmd, t_env *env, int *status)
{
	char	**envp;
	char	*path;

	envp = env_to_envp(env);
	if (!envp)
	{
		perror("env_to_envp");
		*status = 1;
		return (1);
	}
	path = find_cmd_path(cmd->args[0], envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		execve(cmd->args[0], cmd->args, envp);
        perror(cmd->args[0]);
		free_split(envp);
		*status = 127;
		return (127);
	}
	*status = fork_and_wait(cmd, path, env);
	free_split(envp);
	free(path);
	return (0);
}

