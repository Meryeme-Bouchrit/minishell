/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:24:24 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/27 09:32:32 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// Creates a t_env* linked list from a char **envp array
t_env	*copy_env(char **envp)
{
	t_env	*head;
	t_env	*node;
	char	*equal;
	char	*key;
	char	*value;

	head = NULL;
	while (*envp)
	{
		equal = ft_strchr(*envp, '=');
		if (equal)
		{
			key = ft_substr(*envp, 0, equal - *envp);
			value = ft_strdup(equal + 1);
		}
		else
		{
			key = ft_strdup(*envp);
			value = NULL;
		}
		node = malloc(sizeof(t_env));
		if (!node)
			return (free_env_list(head), NULL);
		node->key = key;
		node->value = value;
		node->next = NULL;
		add_node_back(&head, node);
		envp++;
	}
	return (head);
}

// Get value by key, or NULL if not found
char	*get_env_value(t_env *env, const char *key)
{
	t_env	*node = find_env_node(env, key);
	if (node)
	return (node->value);
	return (NULL);
}

// Sets or updates a key=value in the linked list environment
void	set_env_var(t_env **env, const char *key, const char *value)
{
	t_env	*cur;

	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return;
		}
		cur = cur->next;
	}
	add_env(env, (char *)key, (char *)value);
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