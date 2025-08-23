/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lookup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:51:06 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 18:21:30 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

t_env	*env_find(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*env_get(t_env *env, const char *key)
{
	t_env	*n;

	n = env_find(env, key);
	if (n)
		return (n->value);
	return (NULL);
}

void	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*cur;
	char	*val_to_set;

	cur = env_find(*env, key);
	if (value)
		val_to_set = ft_strdup(value);
	else
		val_to_set = ft_strdup("");
	if (cur)
	{
		free(cur->value);
		cur->value = val_to_set;
		return ;
	}
	env_add(env, ft_strdup(key), val_to_set);
}

void	env_unset(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}
