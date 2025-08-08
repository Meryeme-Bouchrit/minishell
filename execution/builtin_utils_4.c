/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:00:49 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/07 02:57:03 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// Returns the length of a NULL-terminated env array
int	env_len(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

// Frees a NULL-terminated array of strings
void free_split(char **split)
{
	int i;
	i = 0;
	if (!split)
		return;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

// Removes surrounding quotes from a string if present
char *ft_remove_quotes(char *str)
{
	size_t len;
	char *new;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"') ||
		(str[0] == '\'' && str[len - 1] == '\''))
	{
		new = ft_substr(str, 1, len - 2);
		return (new);
	}
	return (ft_strdup(str));
}

// Compares two strings like strcmp, returns diff at first mismatch
int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
