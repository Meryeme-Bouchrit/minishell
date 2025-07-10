/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:52:12 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/04 20:21:14 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_cmd(char **argv, char **envp)
{
	pid_t pid;
	int status;

	pid = fork();
	if(pid == 0)
	{
		if(execve(argv[0], argv, envp) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	else if(pid > 0)
	{
		waitpid(pid, &status, 0);
	}
	else
	{
		perror("fork");
		return(1);
	}
	return(0);
}

// PATH=/usr/local/bin:/usr/bin:/bin

char *get_command_path(char *cmd, char **envp)
{
	int i;
	char *path_value;

	i = 0;
	while(envp[i] != NULL)
	{
		if(strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_value = envp[i] + 5;
			ft_split(path_value, ':');
		}
		i++;
	}
}