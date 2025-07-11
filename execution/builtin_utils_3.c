/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:19:29 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/11 18:45:20 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Add a node at the end of the env list
void	add_node_back(t_env **env, t_env *new)
{
	t_env	*cur;

	if (!*env)
	{
		*env = new;
		return;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

// Free entire env linked list
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

// Find node by key, return pointer or NULL if not found
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

// Removes the key from env linked list if it exists
void	unset_env_var(t_env **env, char *key)
{
	t_env	*cur = *env;
	t_env	*prev = NULL;

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
			return;
		}
		prev = cur;
		cur = cur->next;
	}
}

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
