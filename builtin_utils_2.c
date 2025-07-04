/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:22:14 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/04 13:05:12 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Adds a key=value pair to the environment array
void	add_env(char ***env, char *key, char *value)
{
	char *tmp;
	char *new_var;

	tmp = ft_strjoin(key, "=");
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_var)
		return;
	*env = realloc_env(*env, new_var);
}

// Replaces key if it exists, otherwise adds it to the environment
void replace_env(char ***env, char *key, char *value)
{
	int		i;
	char	*new_var;
	char	*tmp;
	tmp = ft_strjoin(key, "=");
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_var)
		return;
	i = 0;
	while ((*env)[i])
	{
		if (!ft_strncmp((*env)[i], key, ft_strlen(key)) && (*env)[i][ft_strlen(key)] == '=')
		{
			free((*env)[i]);
			(*env)[i] = new_var;
			return;
		}
		i++;
	}
	*env = realloc_env(*env, new_var);
}

// Checks whether a key already exists in the environment
int env_has_key(char **env, char *key)
{
	int i;
	int key_len;

	if (!env || !key)
		return (0);
	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
			return (1);
		i++;
	}
	return (0);
}

// Updates env if key exists or adds it if it doesn’t
void handle_env_update(char ***env, char *key, char *value)
{
	if (value)
		replace_env(env, key, value);
	else if (!env_has_key(*env, key))
		add_env(env, key, "");
}

// Splits "key=value" into separate key and value strings
void parse_export_arg(char *arg, char **key, char **value)
{
	char *equal;
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}
