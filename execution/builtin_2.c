/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:16:14 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 10:24:55 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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

int	ft_unset(char **args, t_env **env)
{
	int	j;
	int	had_error;

	had_error = 0;
	j = 1;
	while (args[j])
	{
		if (!is_valid_key(args[j]))
		{
			write(2, "minishell: unset: ", 19);
			write(2, args[j], ft_strlen(args[j]));
			write(2, ": not a valid identifier\n", 26);
			had_error = 1;
		}
		else
		{
			unset_env_var(env, args[j]);
		}
		j++;
	}
	return (had_error);
}

static void	print_export_error(char *arg)
{
	write(2, "minishell: export: `", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
}

static int	process_export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		err;

	err = 0;
	parse_export_arg(arg, &key, &value);
	if (!is_valid_key(key))
	{
		print_export_error(arg);
		err = 1;
	}
	else
		env_update(env, key, value);
	free(key);
	if (value)
		free(value);
	return (err);
}

int	ft_export(char **args, t_env **env)
{
	int	i;
	int	had_error;

	i = 1;
	had_error = 0;
	if (!args[1])
	{
		print_export_env(*env);
		return (0);
	}
	while (args[i])
	{
		if (process_export_arg(args[i], env))
			had_error = 1;
		i++;
	}
	return (had_error);
}
