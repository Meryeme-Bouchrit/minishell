/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:23:29 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/22 12:31:16 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**env_to_envp(t_env *env)
{
	int		len;
	t_env	*cur;
	char	**envp;
	char	*tmp;
	int		i;

	len = 0;
	cur = env;
	while (cur)
	{
		len++;
		cur = cur->next;
	}
	envp = malloc(sizeof(char *) * (len + 1));
	if (!envp)
		return (NULL);
	i = 0;
	cur = env;
	while (cur)
	{
		tmp = ft_strjoin(cur->key, "=");
		if (!tmp)
		{
			while (i--)
				free(envp[i]);
			free(envp);
			return (NULL);
		}
		envp[i] = ft_strjoin(tmp, cur->value);
		free(tmp);
		if (!envp[i])
		{
			while (i--)
				free(envp[i]);
			free(envp);
			return (NULL);
		}
		i++;
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}

t_env	*dup_env(char **envp)
{
	t_env	*env;
	char	*eq;
	char	*key;
	char	*value;

	env = NULL;
	while (*envp)
	{
		eq = ft_strchr(*envp, '=');
		if (eq)
		{
			key = malloc(eq - *envp + 1);
			if (!key)
				return (NULL);
			ft_memcpy(key, *envp, eq - *envp);
			key[eq - *envp] = '\0';
			value = ft_strdup(eq + 1);
			add_env(&env, key, value);
		}
		envp++;
	}
	return (env);
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
