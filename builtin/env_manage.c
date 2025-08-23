/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:04:42 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 18:22:03 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

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

void	env_add(t_env **env, char *key, char *value)
{
	t_env	*n;
	t_env	*cur;

	n = env_new(key, value);
	if (!n)
		return ;
	if (!*env)
	{
		*env = n;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = n;
}

void	env_free_all(t_env *env)
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

t_env	*env_from_entry(char *envp_entry)
{
	t_env	*n;
	char	*eq;

	eq = ft_strchr(envp_entry, '=');
	if (eq)
		n = env_new(ft_substr(envp_entry, 0, eq - envp_entry),
				ft_strdup(eq + 1));
	else
		n = env_new(ft_strdup(envp_entry), NULL);
	return (n);
}

t_env	*env_dup(char **envp)
{
	t_env	*env;
	t_env	*n;

	env = NULL;
	while (*envp)
	{
		n = env_from_entry(*envp);
		if (!n)
		{
			env_free_all(env);
			return (NULL);
		}
		env_add(&env, n->key, n->value);
		free(n);
		envp++;
	}
	return (env);
}
