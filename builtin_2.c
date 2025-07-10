/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:16:14 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/10 10:18:31 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Exits the shell with status, handles numeric args and too many args
int	ft_exit(char **args, int *exit_status)
{
	int		n;
	char	*cleaned_arg;

	write(1, "exit\n", 5);
	if (!args[1])
		exit(0);
	cleaned_arg = ft_remove_quotes(args[1]);
	if (!ft_is_numeric(cleaned_arg))
	{
		write(2, "exit: numeric argument required\n", 33);
		free(cleaned_arg);
		exit(255);
	}
	if (args[2])
	{
		write(2, "exit: too many arguments\n", 26);
		free(cleaned_arg);
		*exit_status = 1;
		return (1);
	}
	n = ft_atoi(cleaned_arg);
	free(cleaned_arg);
	exit(n % 256);
}

// Removes each variable name passed in args from the environment
int	ft_unset(char **args, t_env **env)
{
	int	j;

	j = 1;
	while (args[j])
	{
		unset_env_var(env, args[j]);
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
		return (print_export_env(*env), 0);
	i = 1;
	while (args[i])
	{
		parse_export_arg(args[i], &key, &value);
		if (!is_valid_key(key))
			printf("minishell: export: `%s`: not a valid identifier\n", args[i]);
		else
			handle_env_update(env, key, value);
		free(key);
		if (value)
			free(value);
		i++;
	}
	return (0);
}
