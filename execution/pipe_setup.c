/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:57 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 23:57:45 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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

void	setup_child(int **pipes, int idx, int total)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (idx > 0 && dup2(pipes[idx - 1][0], 0) == -1)
	{
		perror("dup2");
		exit(1);
	}
	if (idx < total - 1 && dup2(pipes[idx][1], 1) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close_all_pipes(pipes, total - 1);
}

int	print_error(char *cmd, char *msg, int code, int *status)
{
	ft_putstr_fd("minishell: ", 2);
	write(2, cmd, ft_strlen(cmd));
	write(2, msg, ft_strlen(msg));
	*status = code;
	return (code);
}

int	check_cmd_errors(char *cmd, int *status)
{
	char	*new_path;
	int		ret;

	new_path = ft_strjoin(cmd, "/");
	if (access(new_path, F_OK) == 0)
	{
		free(new_path);
		ret = print_error(cmd, ": Is a directory\n", 126, status);
		return (ret);
	}
	free(new_path);
	if (access(cmd, F_OK) != 0)
	{
		ret = print_error(cmd, ": No such file or directory\n", 127, status);
		return (ret);
	}
	if (access(cmd, X_OK) != 0)
	{
		ret = print_error(cmd, ": Permission denied\n", 126, status);
		return (ret);
	}
	return (0);
}
