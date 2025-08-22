/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:58:35 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/21 14:20:05 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	process_export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		err = 0;

	builtin_parse_export_arg(arg, &key, &value);
	if (!builtin_is_valid_key(key))
	{
		write(2, "minishell: export: `", 21);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 27);
		err = 1;
	}
	else
		env_set(env, key, value ? value : "");
	free(key);
	if (value)
		free(value);
	return (err);
}

int	ft_export(char **args, t_env **env)
{
	int	i = 1;
	int	had_error = 0;

	if (!args[1])
	{
		builtin_print_export_env(*env);
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

int	ft_unset(char **args, t_env **env)
{
	int	j = 1;
	int	had_error = 0;

	while (args[j])
	{
		if (!builtin_is_valid_key(args[j]))
		{
			write(2, "minishell: unset: ", 19);
			write(2, args[j], ft_strlen(args[j]));
			write(2, ": not a valid identifier\n", 26);
			had_error = 1;
		}
		else
			env_unset(env, args[j]);
		j++;
	}
	return (had_error);
}
