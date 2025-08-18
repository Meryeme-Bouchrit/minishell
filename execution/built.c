/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:47:33 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 10:47:33 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int only_spaces(const char *str)
{
    while (*str)
    {
        if (*str != ' ' && *str != '\t')
            return (0);
        str++;
    }
    return (1);
}

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "echo") == 0)
        return (1);
    if (ft_strcmp(cmd, "cd") == 0)
        return (1);
    if (ft_strcmp(cmd, "pwd") == 0)
        return (1);
    if (ft_strcmp(cmd, "export") == 0)
        return (1);
    if (ft_strcmp(cmd, "unset") == 0)
        return (1);
    if (ft_strcmp(cmd, "env") == 0)
        return (1);
    if (ft_strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}

int run_builtin(char **args, t_env **env, int *st)
{
    if (!args || !args[0])
        return (0);
    if (!ft_strcmp(args[0], "exit"))
    {
        ft_exit(args, st);
        return (-1);
    }
    else if (!ft_strcmp(args[0], "cd"))
        *st = ft_cd(args, env);
    else if (!ft_strcmp(args[0], "export"))
        *st = ft_export(args, env);
    else if (!ft_strcmp(args[0], "unset"))
        *st = ft_unset(args, env);
    else if (!ft_strcmp(args[0], "echo"))
        *st = ft_echo(args);
    else if (!ft_strcmp(args[0], "pwd"))
        *st = ft_pwd();
    else if (!ft_strcmp(args[0], "env"))
        *st = ft_env(*env);
    else
        return (0);
    return (1);
}
