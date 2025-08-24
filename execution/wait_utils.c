/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:39:06 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/24 18:20:42 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int wait_last(pid_t *pids, int n)
{
    int i = 0;
    int status;
    int last_status = 0;

    while (i < n)
    {
        waitpid(pids[i], &status, 0);
        last_status = status_to_exit(status);
        i++;
    }
    if (last_status == 130)
        write(1, "\n", 1);
    else if (last_status == 131)
        write(2, "Quit (core dumped)\n", 20);

    return last_status;
}

int	status_to_exit(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
