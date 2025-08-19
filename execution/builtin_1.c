/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:31:35 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 11:18:47 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*get_home(t_env *env)
{
	char	*path;

	path = env_get(env, "HOME");
	if (!path)
	{
		write(2, "minishell: cd: HOME not set\n", 28);
		return (NULL);
	}
	return (path);
}

static char	*get_oldpwd(t_env *env)
{
	char	*path;

	path = env_get(env, "OLDPWD");
	if (!path)
	{
		write(2, "minishell: cd: OLDPWD not set\n", 30);
		return (NULL);
	}
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	return (path);
}

static int	change_dir(char *path, t_env **env_list, char *old_path,
		char *new_path)
{
	if (chdir(path) != 0)
	{
		write(2, "minishell: cd: ", 15);
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
		perror("");
		free(old_path);
		return (1);
	}
	new_path = getcwd(NULL, 0);
	if (!new_path)
	{
		perror("minishell: cd");
		free(old_path);
		return (1);
	}
	env_set(env_list, "OLDPWD", old_path);
	env_set(env_list, "PWD", new_path);
	free(old_path);
	free(new_path);
	return (0);
}

int	ft_cd(char **args, t_env **env_list)
{
	char	*path;
	char	*old_dir;
	char	*new_dir;

	if (args[1] && args[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 1);
	path = args[1];
	if (!path || ft_strcmp(path, "~") == 0)
		path = get_home(*env_list);
	else if (ft_strcmp(path, "-") == 0)
		path = get_oldpwd(*env_list);
	if (!path)
		return (1);
	old_dir = getcwd(NULL, 0);
	if (!old_dir)
		return (perror("minishell: cd"), 1);
	new_dir = NULL;
	return (change_dir(path, env_list, old_dir, new_dir));
}

int	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("pwd");
		return (1);
	}
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	return (0);
}

int	ft_env(t_env *env_list)
{
	while (env_list)
	{
		if (env_list->value)
		{
			write(1, env_list->key, ft_strlen(env_list->key));
			write(1, "=", 1);
			write(1, env_list->value, ft_strlen(env_list->value));
			write(1, "\n", 1);
		}
		env_list = env_list->next;
	}
	return (0);
}
