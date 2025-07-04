/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:35:07 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/04 13:05:15 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints environment variables in export format with quotes
void print_export_env(char **env)
{
	int		i;
	int		key_len;
	char	*equal;

	i = 0;
	while (env[i])
	{
		write(1, "declare -x ", 11);
		equal = ft_strchr(env[i], '=');
		if (equal)
		{
			key_len = equal - env[i];
			write(1, env[i], key_len);
			write(1, "=\"", 2);
			write(1, equal + 1, ft_strlen(equal + 1));
			write(1, "\"", 1);
		}
		else
			write(1, env[i], ft_strlen(env[i]));
		write(1, "\n", 1);
		i++;
	}
}

// Verifies that a variable key name is valid (alphanum + underscore)
int is_valid_key(char *key)
{
	int i;

	if (!key || !key[0])
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Creates a new "key=value" string for use in env arrays
char *create_env_var(const char *key, const char *value)
{
	char *tmp;
	char *full;
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, value);
	free(tmp);
	return (full);
}

// Updates or appends a key=value pair in the env array
int update_env_var(char **env, char *key, char *value)
{
	int		i;
	char	*full;

	i = 0;
	full = create_env_var(key, value);
	if (!full)
		return (1);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, ft_strlen(key)) && env[i][ft_strlen(key)] == '=')
		{
			free(env[i]);
			env[i] = full;
			return (0);
		}
		i++;
	}
	env = realloc_env(env, full);
	if (!env)
	{
		free(full);
		return (1);
	}
	return (0);
}
