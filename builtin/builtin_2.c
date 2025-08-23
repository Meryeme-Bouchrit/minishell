/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:58:35 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 18:56:29 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	process_export_arg(char *arg, t_env **envp)
{
	char	*key;
	char	*value;
	int		err;

	err = 0;
	parse_export_arg(arg, &key, &value);
	if (!is_valid_key(key))
	{
		write(2, "minishell: export: `", 20);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 27);
		err = 1;
	}
	else
	{
		if (value)
			env_set(envp, key, value);
		else
			env_set(envp, key, "");
	}
	free(key);
	if (value)
		free(value);
	return (err);
}

int	ft_export(char **args, t_env **envp)
{
	int	i;
	int	had_error;

	i = 1;
	had_error = 0;
	if (!args[1])
	{
		print_export_env(*envp);
		return (0);
	}
	while (args[i])
	{
		if (process_export_arg(args[i], envp))
			had_error = 1;
		i++;
	}
	return (had_error);
}

int	ft_unset(char **args, t_env **env)
{
	int	j;
	int	had_error;

	j = 1;
	had_error = 0;
	while (args[j])
	{
		if (!is_valid_key(args[j]))
		{
			write(2, "minishell: unset: ", 18);
			write(2, args[j], ft_strlen(args[j]));
			write(2, ": not a valid identifier\n", 25);
			had_error = 1;
		}
		else
			env_unset(env, args[j]);
		j++;
	}
	return (had_error);
}
