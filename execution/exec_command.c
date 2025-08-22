/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:01:59 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/22 12:31:00 by mbouchri         ###   ########.fr       */
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
		err = check_directory(cmd->args[0], status);
		if (err != 0)
			return (err);
		err = check_access_errors(cmd->args[0], status);
		if (err != 0)
			return (err);
	}
	err = resolve_and_exec(cmd, env, status);
	if (err != 0)
		return (err);
	return (0);
}

int	fork_and_wait(t_cmd *cmd, char *path, t_env *env)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		child_process_single(cmd, path, env);
		return (0);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
	ret = get_exit_status(status);
	return (ret);
}

int	check_directory(char *cmd, int *status)
{
	int	fd;

	fd = open(cmd, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Is a directory\n", 18);
		*status = 126;
		return (126);
	}
	return (0);
}

int	check_access_errors(char *cmd, int *status)
{
	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) != 0)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Permission denied\n", 21);
		*status = 126;
		return (126);
	}
	if (access(cmd, F_OK) != 0)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": No such file or directory\n", 29);
		*status = 127;
		return (127);
	}
	return (0);
}

int	resolve_and_exec(t_cmd *cmd, t_env *env, int *status)
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
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, ": command not found\n", 21);
		free_split(envp);
		*status = 127;
		return (127);
	}
	*status = fork_and_wait(cmd, path, env);
	free_split(envp);
	free(path);
	return (0);
}
