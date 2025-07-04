/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:22:10 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/04 13:05:21 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
