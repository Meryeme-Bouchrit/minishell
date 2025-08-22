/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:56:25 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/21 14:19:59 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	check_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

int	ft_echo(char **args)
{
	int	i = 1;
	int	newline = 1;

	while (args[i] && check_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

int	ft_pwd(void)
{
	char	*path = getcwd(NULL, 0);

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
