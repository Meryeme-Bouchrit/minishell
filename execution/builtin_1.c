/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:31:35 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/11 18:07:02 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Changes the current working directory and updates PWD and OLDPWD
int	ft_cd(char **args, t_env **env)
{
	char	*old_directory;
	char	*new_directory;
	char	*path;

	if (!args[1])
		return (write(2, "cd: missing argument\n", 22), 1);
	path = ft_remove_quotes(args[1]);
	if (!path)
		return (1);
	old_directory = getcwd(NULL, 0);
	if (!old_directory)
		return (perror("cd: getcwd"), free(path), 1);
	if (chdir(path) != 0)
		return (perror("cd"), free(old_directory), free(path), 1);
	new_directory = getcwd(NULL, 0);
	if (!new_directory)
		return (perror("cd: getcwd"), free(old_directory), free(path), 1);
	set_env_var(env, "OLDPWD", old_directory);
	set_env_var(env, "PWD", new_directory);
	free(old_directory);
	free(new_directory);
	free(path);
	return (0);
}

// Prints the current working directory to stdout
int	ft_pwd(void)
{
	char	*dir;

	dir = getcwd(NULL, 0);
	if (!dir)
		return (perror("pwd"), 1);
	write(1, dir, ft_strlen(dir));
	write(1, "\n", 1);
	free(dir);
	return (0);
}

// Displays all environment variables that have a value
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
