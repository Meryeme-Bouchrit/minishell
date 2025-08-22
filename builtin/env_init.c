/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:05:05 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/21 14:20:30 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

t_env	*env_new_from_entry(char *envp_entry)
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
		n = env_new_from_entry(*envp);
		if (!n)
		{
			free_env_list(env);
			return (NULL);
		}
		add_env(&env, n->key, n->value);
		free(n);
		envp++;
	}
	return (env);
}
