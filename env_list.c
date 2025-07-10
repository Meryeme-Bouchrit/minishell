/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:19:29 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/10 09:24:01 by mbouchri         ###   ########.fr       */
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

// Get value by key, or NULL if not found
char	*get_env_value(t_env *env, const char *key)
{
	t_env	*node = find_env_node(env, key);
	if (node)
		return (node->value);
	return (NULL);
}
