/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:21:01 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/11 04:06:48 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	child_process_single(t_cmd *cmd, char *path, t_env *env)
{
	char	**envp_arr;

	ft_handle_redirs(cmd->io_fds, env);
	envp_arr = env_to_envp(env);
	if (execve(path, cmd->args, envp_arr) == -1)
	{
		perror("execve");
		free_split(envp_arr);
		exit(127);
	}
}

int	fork_and_wait(t_cmd *cmd, char *path, t_env *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
        child_process_single(cmd, path, env);
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
	{
		perror("fork");
		return (1);
	}
	return (0);
}

int exec_cmd(t_cmd *cmd, t_env *env)
{
    char *path;
    char **envp;

    envp = env_to_envp(env);
    if (!envp)
    {
        perror("env_to_envp");
        exit(1);
    }
    path = find_cmd_path(cmd->args[0], envp);
    if (!path)
    {
        free_split(envp);
        perror(cmd->args[0]);
        exit(127);
    }
    ft_handle_redirs(cmd->io_fds, env);
    execve(path, cmd->args, envp);
    perror("execve");
    free(path);
    free_split(envp); 
    exit(127);
}
