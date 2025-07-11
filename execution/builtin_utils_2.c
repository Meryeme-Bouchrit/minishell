/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:22:14 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/11 18:44:39 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Splits "key=value" into separate key and value strings
void	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
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


// Checks if the string is a valid numeric argument (with optional sign)
int ft_is_numeric(const char *str)
{
	int i = 0;
	char sign = 0;

	if (!str || !str[0])
		return (0);

	while (str[i] == '+' || str[i] == '-')
	{
		if (sign && str[i] != sign)
			return (0); 
		sign = str[i];
		i++;
	}
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
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
