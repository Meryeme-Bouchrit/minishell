/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:39:06 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 19:39:22 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
		last_status = status_to_exit(status);
		i++;
	}
	return (last_status);
}

int	status_to_exit(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			return (130);
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			write(2, "Quit\n", 5);
			return (131);
		}
		return (128 + WTERMSIG(status));
	}
	return (1);
}
