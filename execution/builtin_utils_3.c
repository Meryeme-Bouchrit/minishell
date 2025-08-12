/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:19:29 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/12 15:25:24 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	add_node_back(t_env **env, t_env *new)
{
	t_env	*cur;

	if (!*env)
	{
		*env = new;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

t_env	*find_env_node(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	unset_env_var(t_env **env, char *key)
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

void	add_env(t_env **env, char *key, char *value)
{
	t_env	*node;
	t_env	*cur;

	node = add_env_node(key, value);
	if (!node)
		return ;
	if (!*env)
	{
		*env = node;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}
