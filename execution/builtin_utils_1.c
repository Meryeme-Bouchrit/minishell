/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:24:24 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/16 15:09:39 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_env	*create_env_node(char *envp_entry, t_env **head)
{
	t_env	*node;
	char	*equal;

	equal = ft_strchr(envp_entry, '=');
	if (equal)
		node = add_env_node(ft_substr(envp_entry, 0, equal - envp_entry),
				ft_strdup(equal + 1));
	else
		node = add_env_node(ft_strdup(envp_entry), NULL);
	if (!node)
	{
		free_env_list(*head);
		return (NULL);
	}
	return (node);
}

t_env	*copy_env(char **envp)
{
	t_env	*head;
	t_env	*node;

	head = NULL;
	while (*envp)
	{
		node = create_env_node(*envp, &head);
		if (!node)
			return (NULL);
		add_node_back(&head, node);
		envp++;
	}
	return (head);
}

char	*get_env_value(t_env *env, const char *key)
{
	t_env	*node;

	node = find_env_node(env, key);
	if (node)
		return (node->value);
	return (NULL);
}

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
			return ;
		}
		cur = cur->next;
	}
	add_env(env, ft_strdup(key), ft_strdup(value));
}

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

void	replace_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			if (value)
				tmp->value = ft_strdup(value);
			else
				tmp->value = ft_strdup("");
			return ;
		}
		tmp = tmp->next;
	}
	if (value)
		add_env(env, ft_strdup(key), ft_strdup(value));
	else
		add_env(env, ft_strdup(key), ft_strdup(""));
}

void	handle_env_update(t_env **env, char *key, char *value)
{
	if (value)
		replace_env(env, key, value);
	else if (!env_has_key(*env, key))
		add_env(env, ft_strdup(key), ft_strdup(""));
}

t_env	*add_env_node(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}
