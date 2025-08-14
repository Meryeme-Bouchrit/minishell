/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:31:35 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/14 08:54:15 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int ft_cd(char **args, t_env **env)
{
    char *old_directory;
    char *new_directory;
    char *path;

    if (args[1] && args[2])
    {
        write(2, "minishell: cd: too many arguments\n", 34);
        return (1);
    }
    path = args[1];
    if (!path || ft_strcmp(path, "~") == 0)
    {
        path = get_env_value(*env, "HOME");
        if (!path)
        {
            write(2, "minishell: cd: HOME not set\n", 28);
            return (1);
        }
    }
    else if (ft_strcmp(path, "-") == 0)
    {
        path = get_env_value(*env, "OLDPWD");
        if (!path)
        {
            write(2, "minishell: cd: OLDPWD not set\n", 30);
            return (1);
        }
        write(1, path, ft_strlen(path));
        write(1, "\n", 1);
    }
    old_directory = getcwd(NULL, 0);
    if (!old_directory)
    {
        perror("minishell: cd");
        return (1);
    }
    if (chdir(path) != 0)
    {
        write(2, "minishell: cd: ", 15);
        write(2, path, ft_strlen(path));
        write(2, ": ", 2);
        perror("");
        free(old_directory);
        return (1);
    }
    new_directory = getcwd(NULL, 0);
    if (!new_directory)
    {
        perror("minishell: cd");
        free(old_directory);
        return (1);
    }
    set_env_var(env, "OLDPWD", old_directory);
    set_env_var(env, "PWD", new_directory);
    free(old_directory);
    free(new_directory);
    return (0);
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
