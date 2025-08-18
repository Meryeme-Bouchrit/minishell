/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:31:35 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 09:15:01 by mbouchri         ###   ########.fr       */
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

static int	change_dir(char *path, t_env **env, char *old_dir, char *new_dir)
{
	if (chdir(path) != 0)
	{
		write(2, "minishell: cd: ", 15);
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
		perror("");
		free(old_dir);
		return (1);
	}
	new_dir = getcwd(NULL, 0);
	if (!new_dir)
	{
		perror("minishell: cd");
		free(old_dir);
		return (1);
	}
	env_set(env, "OLDPWD", old_dir);
	env_set(env, "PWD", new_dir);
	free(old_dir);
	free(new_dir);
	return (0);
}

int	ft_cd(char **args, t_env **env)
{
	char	*path;
	char	*old_dir;
	char	*new_dir;

	if (args[1] && args[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 1);
	path = args[1];
	if (!path || ft_strcmp(path, "~") == 0)
		path = get_home(*env);
	else if (ft_strcmp(path, "-") == 0)
		path = get_oldpwd(*env);
	if (!path)
		return (1);
	old_dir = getcwd(NULL, 0);
	if (!old_dir)
		return (perror("minishell: cd"), 1);
	new_dir = NULL;
	return (change_dir(path, env, old_dir, new_dir));
}

int	ft_pwd(void)
{
	char	*dir;

	dir = getcwd(NULL, 0);
	if (!dir)
	{
		perror("pwd");
		return (1);
	}
	write(1, dir, ft_strlen(dir));
	write(1, "\n", 1);
	free(dir);
	return (0);
}

int	ft_env(t_env *env)
{
	while (env)
	{
		if (env->value)
		{
			write(1, env->key, ft_strlen(env->key));
			write(1, "=", 1);
			write(1, env->value, ft_strlen(env->value));
			write(1, "\n", 1);
		}
		env = env->next;
	}
	return (0);
}
