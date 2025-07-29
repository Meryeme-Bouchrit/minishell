/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:16:14 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/29 12:11:37 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// Exits the shell with status, handles numeric args and too many args
int ft_exit(char **args, int *exit_status)
{
    write(1, "exit\n", 5);
    if (!args || !args[1])
    {
        *exit_status = 0;
        return (-1); // signal to exit shell
    }
    if (!ft_is_numeric(args[1]))
    {
        write(2, "exit: ", 6);
        write(2, args[1], ft_strlen(args[1]));
        write(2, ": numeric argument required\n", 29);
        *exit_status = 255;
        return (-1);
    }
    if (args[2])
    {
        write(2, "exit: too many arguments\n", 26);
        *exit_status = 1;
        return (1); // error: do NOT exit shell
    }
    *exit_status = ft_atoi(args[1]) % 256;
    return (-1); // signal to exit shell
}

// Removes each variable name passed in args from the environment
int	ft_unset(char **args, t_env **env)
{
	int	j;

	j = 1;
	while (args[j])
	{
		if (!is_valid_key(args[j]))
		{
			write(2, "minishell: unset: ", 19);
			write(2, args[j], ft_strlen(args[j]));
			write(2, ": not a valid identifier\n", 26);
		}
		else
		{
			unset_env_var(env, args[j]);
		}
		j++;
	}
	return (0);
}


// Adds or updates environment variables, or prints them sorted
int	ft_export(char **args, t_env **env)
{
	int		i;
	char	*key;
	char	*value;

	if (!args[1])
	{
		print_export_env(*env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		parse_export_arg(args[i], &key, &value);
		if (!is_valid_key(key))
		{
			write(2, "minishell: export: ", 20);
			write(2, args[i], ft_strlen(args[i]));
			write(2, ": not a valid identifier\n", 26);
		}
		else
		{
			handle_env_update(env, key, value);
		}
		free(key);
		if (value)
			free(value);
		i++;
	}
	return (0);
}

