/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:24:24 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/04 13:05:09 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Compares two strings like strcmp, returns diff at first mismatch
int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

// Prints echo arguments starting from index i, space-separated
void	print_echo_args(char **args, int i)
{
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
}

// Handles echo builtin with support for -n flag (no newline)
int	ft_echo(char **args)
{
	int	i;
	int	newline;
	int	j;

	i = 1;
	newline = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		newline = 0;
		i++;
	}
	print_echo_args(args, i);
	if (newline)
		write(1, "\n", 1);
	return (0);
}


// Removes the key from env if it exists, shifting the array
void	unset_env_var(char **env, char *key)
{
	int	i = 0;
	int	k;
	int	len = ft_strlen(key);

	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
		{
			free(env[i]);
			k = i;
			while (env[k])
			{
				env[k] = env[k + 1];
				k++;
			}
			break;
		}
		i++;
	}
}

