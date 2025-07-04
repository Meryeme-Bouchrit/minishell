/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:31:51 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/04 13:05:19 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns the length of a NULL-terminated env array
int env_len(char **envp)
{
	int i;
	i = 0;
	while (envp[i])
		i++;
	return (i);
}

// Duplicates a NULL-terminated env array in heap memory
char **copy_env(char **envp)
{
	int i;
	char **new_env;
	int len;

	i = 0;
	len = env_len(envp);
	new_env = malloc(sizeof(char *) * (len + 1));
	if (!new_env)
		return (NULL);
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			while (i-- > 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

// Adds a new env var string to an existing env array
char **realloc_env(char **env, char *new_var)
{
	int		i;
	char	**new_env;

	if (!env || !new_var)
		return (NULL);
	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i++] = new_var;
	new_env[i] = NULL;
	free(env);
	return (new_env);
}
