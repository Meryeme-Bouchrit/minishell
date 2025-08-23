/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:56:19 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 18:58:25 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

char	*get_home(t_env *envp)
{
	char	*path;

	path = env_get(envp, "HOME");
	if (!path)
		write(2, "minishell: cd: HOME not set\n", 28);
	return (path);
}

char	*get_oldpwd(t_env *envp)
{
	char	*path;

	path = env_get(envp, "OLDPWD");
	if (!path)
		write(2, "minishell: cd: OLDPWD not set\n", 30);
	else
	{
		write(1, path, ft_strlen(path));
		write(1, "\n", 1);
	}
	return (path);
}

int	change_dir(char *path, t_env **envp, char *oldpwd)
{
	char	*new_path;

	if (chdir(path) != 0)
	{
		write(2, "minishell: cd: ", 15);
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
		perror("");
		free(oldpwd);
		return (1);
	}
	new_path = getcwd(NULL, 0);
	if (!new_path)
	{
		perror("minishell: cd");
		free(oldpwd);
		return (1);
	}
	env_set(envp, "OLDPWD", oldpwd);
	env_set(envp, "PWD", new_path);
	free(oldpwd);
	free(new_path);
	return (0);
}

int	ft_cd(char **args, t_env **envp)
{
	char	*path;
	char	*old_dir;

	if (args[1] && args[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	path = args[1];
	if (!path || ft_strcmp(path, "~") == 0)
		path = get_home(*envp);
	else if (ft_strcmp(path, "-") == 0)
		path = get_oldpwd(*envp);
	if (!path)
		return (1);
	old_dir = getcwd(NULL, 0);
	if (!old_dir)
	{
		perror("minishell: cd");
		return (1);
	}
	return (change_dir(path, envp, old_dir));
}

int	ft_exit(char **args, int *status)
{
	write(1, "exit\n", 5);
	if (!args[1])
		return (-1);
	if (!ft_is_numeric(args[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 29);
		*status = 2;
		return (-1);
	}
	if (args[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		*status = 1;
		return (1);
	}
	*status = (unsigned char)ft_atoi(args[1]);
	return (-1);
}
