/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:19:44 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 18:59:03 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	only_spaces(const char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}

int	is_builtin(char *cmd)
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

int	run_builtin(char **args, t_env **envp, int *status)
{
	if (!args || !args[0])
		return (0);
	if (!ft_strcmp(args[0], "exit"))
	{
		ft_exit(args, status);
		return (-1);
	}
	else if (!ft_strcmp(args[0], "cd"))
		*status = ft_cd(args, envp);
	else if (!ft_strcmp(args[0], "export"))
		*status = ft_export(args, envp);
	else if (!ft_strcmp(args[0], "unset"))
		*status = ft_unset(args, envp);
	else if (!ft_strcmp(args[0], "echo"))
		*status = ft_echo(args);
	else if (!ft_strcmp(args[0], "pwd"))
		*status = ft_pwd();
	else if (!ft_strcmp(args[0], "env"))
		*status = ft_env(*envp);
	else
		return (0);
	return (1);
}
