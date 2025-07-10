/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:35:07 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/10 09:14:53 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints environment variables in export format with quotes
void	print_export_env(t_env *env)
{
	while (env)
	{
		write(1, "declare -x ", 11);
		write(1, env->key, ft_strlen(env->key));
		if (env->value)
		{
			write(1, "=\"", 2);
			write(1, env->value, ft_strlen(env->value));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		env = env->next;
	}
}

// Verifies that a variable key name is valid (alphanum + underscore)
int	is_valid_key(char *key)
{
	int	i;

	if (!key || !key[0])
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Creates a new "key=value" string (used internally if needed)
char	*create_env_var(const char *key, const char *value)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, value);
	free(tmp);
	return (full);
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
