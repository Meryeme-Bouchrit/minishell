/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:05:04 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 06:58:08 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int g_exit = 0;

void sigint_prompt(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
    g_exit = 130;
}

void sigquit_prompt(int sig)
{
    (void)sig;
}

void sigint_heredoc(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    exit(130);
}
