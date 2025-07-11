/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:52:12 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/11 17:40:46 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_cmd(char **argv, char **envp)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	cmd_path = get_command_path(argv[0], envp);
	if (cmd_path == NULL)
	{
		perror("cmd");
		return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, argv, envp) == -1)
		{
			free(cmd_path);
			perror("execve");
			exit(1);
		}
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
	}
	else
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}
	return (0);
}


char	*get_command_path(char *cmd, char **envp)
{
	int		i = 0;
	int		j;
	char	*path_value = NULL;
	char	**paths;
	char	*full_path;
	char	*tmp;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_value = envp[i] + 5;
			break;
		}
		i++;
	}
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	j = 0;
	while (paths[j])
	{
		tmp = ft_strjoin(paths[j], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		j++;
	}
	free_split(paths);
	return (NULL);
}
