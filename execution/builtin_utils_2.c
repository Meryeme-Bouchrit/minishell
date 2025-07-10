/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:22:14 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/10 09:14:00 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Adds a new key=value to the environment list
void	add_env(t_env **env, char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return;
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;

	if (!*env)
	{
		*env = new;
		return;
	}
	while ((*env)->next)
		env = &(*env)->next;
	(*env)->next = new;
}

// Replaces key if it exists, otherwise adds it to the environment
void	replace_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return;
		}
		tmp = tmp->next;
	}
	add_env(env, key, value);
}

// Checks whether a key already exists in the environment
int	env_has_key(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

// Updates env if key exists or adds it if it doesn’t
void	handle_env_update(t_env **env, char *key, char *value)
{
	if (value)
		replace_env(env, key, value);
	else if (!env_has_key(*env, key))
		add_env(env, key, "");
}

// Splits "key=value" into separate key and value strings
void	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal;

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
