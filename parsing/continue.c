/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continue_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:48:26 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/28 19:46:54 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	end_len(const char *line, int i)
{
	int		end;
	char	quote;

	end = i;
	while (line[end] && line[end] != ' ' && !is_special(line[end]))
	{
		if (line[end] == '\"' || line[end] == '\'')
		{
			quote = line[end++];
			while (line[end] && line[end] != quote)
				end++;
			if (line[end] == '\0')
			{
				printf("ERROR!\nquotes are not closed!\n\n");
				return (0);
			}
			end++;
		}
		else
			end++;
	}
	return (end);
}

char	*grep_dq(int end, int *start, const char *line)
{
	char	*tmp;
	char	*result;
	int		j;

	tmp = NULL;
	result = NULL;
	(*start)++;
	while ((*start) < end && line[(*start)] && line[(*start)] != '"')
	{
		if (line[(*start)] == '$')
		{
			(*start)++;
			tmp = expand_dollar(line + (*start));
			result = my_strjoin(result, tmp);
			while ((*start) < end && (ft_isalnum(line[(*start)])
					|| line[(*start)] == '_'))
				(*start)++;
		}
		j = (*start);
		while ((*start) < end && line[(*start)] && line[(*start)] != '\"'
			&& line[(*start)] != '$')
			(*start)++;
		result = my_strjoin(result, ft_substr(line, j, (*start) - j));
	}
	return (result);
}

char	*grep_sq(int end, int *start, const char *line)
{
	int		j;
	char	*result;

	j = 0;
	result = NULL;
	(*start)++;
	j = (*start);
	while ((*start) < end && line[(*start)] != '\'')
		(*start)++;
	result = my_strjoin(result, ft_substr(line, j, (*start) - j));
	(*start)++;
	return (result);
}

char	*grep_no_quotes(int end, int *start, const char *line)
{
	char	*result;
	char	*tmp;
	int		j;

	j = 0;
	result = NULL;
	tmp = NULL;
	if (line[(*start)] == '$')
	{
		(*start)++;
		tmp = expand_dollar(line + (*start));
		result = my_strjoin(result, tmp);
		while ((*start) < end && line[(*start)] && line[(*start)] != ' '
			&& (ft_isalnum(line[(*start)]) || line[(*start)] == '_'))
			(*start)++;
	}
	j = (*start);
	while ((*start) < end && line[(*start)] && line[(*start)] != '\"'
		&& line[(*start)] != '\'' && line[(*start)] != '$')
		(*start)++;
	result = my_strjoin(result, ft_substr(line, j, (*start) - j));
	return (result);
}
