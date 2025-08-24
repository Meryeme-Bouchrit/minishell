/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:23:29 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/24 10:51:08 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	env_len(t_env *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

char	*build_env_str(char *key, char *value)
{
	char	*tmp;
	char	*str;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	str = ft_strjoin(tmp, value);
	free(tmp);
	return (str);
}

int	envp_free_on_error(char **envp, int i)
{
	while (--i >= 0)
		free(envp[i]);
	free(envp);
	return (1);
}

char **env_to_envp(t_env *env)
{
    int     len;
    char    **envp;
    t_env   *cur;
    int     i;

    if (!env)
    {
        envp = malloc(sizeof(char *) * 1); // empty array with NULL terminator
        if (!envp)
            return (NULL);
        envp[0] = NULL;
        return envp;
    }
    len = env_len(env);
    envp = malloc(sizeof(char *) * (len + 1));
    if (!envp)
        return (NULL);
    cur = env;
    i = 0;
    while (cur)
    {
        envp[i] = build_env_str(cur->key, cur->value);
        if (!envp[i])
        {
            envp_free_on_error(envp, i);
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
	if (!envp || !*envp)
		return (NULL);
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
			env_add(&env, key, value);
		}
		envp++;
	}
	return (env);
}
