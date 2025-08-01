/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:27:51 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/31 13:02:58 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*expand_dollar(const char *str)
{
	int		i;
	//char	var[256];
	char	*var;
	char	*value;
	char	*result;
	int		vlen;

	i = 0;
	vlen = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	var = malloc(sizeof(char) * i + 1);
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		var[vlen++] = str[i++];
	var[vlen] = '\0';
	value = getenv(var); // get variable from env
	if (!value)
		value = ""; // if var doesn't exist
	// join value + rest of string after var
	result = ft_strdup(value);
	free(var);
	return (result);
}

char	*expand_if_needed(char *token_value, char quote_type)
{
	char	*result;
	char	*ptr;
	char	*tmp;

	if (quote_type == '\'') // single quotes? don't expand
		return (ft_strdup(token_value));
	result = NULL;
	ptr = token_value;
	result = ft_calloc(1, sizeof(char));
	while (*ptr)
	{
		if (*ptr == '$' && *(ptr + 1) && (ft_isalpha(*(ptr + 1)) || *(ptr + 1) == '_'))
		{
			ptr++;
			tmp = expand_dollar(ptr);
			result = ft_strjoin(result, tmp);
				// You can define this helper to join + free
			while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
				ptr++;
		}
		else
		{
			char s[2] = {*ptr, 0};
			tmp = ft_strjoin(result, s);
			free(result);
			result = tmp;
			ptr++;
		}
	}
	return (result);
}
