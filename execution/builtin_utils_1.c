/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:24:24 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 09:28:45 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_env	*new_env(char *envp_entry, t_env **env)
{
	t_env	*n;
	char	*eq;

	eq = ft_strchr(envp_entry, '=');
	if (eq)
		n = env_new(ft_substr(envp_entry, 0, eq - envp_entry), ft_strdup(eq
					+ 1));
	else
		n = env_new(ft_strdup(envp_entry), NULL);
	if (!n)
	{
		free_env_list(*env);
		return (NULL);
	}
	return (n);
}

t_env	*dup_env(char **envp)
{
	t_env	*env;
	t_env	*n;

	env = NULL;
	while (*envp)
	{
		n = new_env(*envp, &env);
		if (!n)
			return (NULL);
		add_node_back(&env, n);
		envp++;
	}
	return (env);
}

char	*env_get(t_env *env, const char *key)
{
	t_env	*n;

	n = find_env_node(env, key);
	if (n)
		return (n->value);
	return (NULL);
}

void	env_set(t_env **env, const char *key, const char *value)
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

void	env_replace(t_env **env, char *key, char *value)
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

void	env_update(t_env **env, char *key, char *value)
{
	if (value)
		env_replace(env, key, value);
	else if (!env_has_key(*env, key))
		add_env(env, ft_strdup(key), ft_strdup(""));
}

t_env	*env_new(char *key, char *value)
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
