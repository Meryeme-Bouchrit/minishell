/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:51:57 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/22 12:34:51 by mbouchri         ###   ########.fr       */
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

void	print_exit(char *cmd, char *msg, int code)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, msg, ft_strlen(msg));
	exit(code);
}

void	check_cmd_errors(t_cmd *cmd)
{
	int	fd;

	if (!cmd->args[0] || !cmd->args[0][0])
		exit(0);
	if (ft_strchr(cmd->args[0], '/'))
	{
		fd = open(cmd->args[0], O_DIRECTORY);
		if (fd != -1)
		{
			close(fd);
			print_exit(cmd->args[0], ": Is a directory\n", 126);
		}
		if (access(cmd->args[0], F_OK) == 0 && access(cmd->args[0], X_OK) != 0)
			print_exit(cmd->args[0], ": Permission denied\n", 126);
		if (access(cmd->args[0], F_OK) != 0)
			print_exit(cmd->args[0], ": No such file or directory\n", 127);
	}
}
