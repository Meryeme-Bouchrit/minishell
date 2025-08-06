/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continue.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:48:26 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/06 03:49:46 by zhassna          ###   ########.fr       */
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
		if (line[end] == '#')
			break ;
		if (line[end] == '\"' || line[end] == '\'')
		{
			quote = line[end++];
			while (line[end] && line[end] != quote)
				end++;
			if (line[end] == '\0')
			{
				printf("ERROR!\nquotes are not closed!\n");
				return (0);
			}
			end++;
		}
		else
			end++;
	}
	return (end);
}

char	*grep_doubleq(t_token *tokens, int end, int *start, const char *line,
		t_env *env)
{
	char	*tmp;
	char	*result;
	int		j;

	(void)tokens;
	tmp = NULL;
	result = NULL;
	(*start)++;
	while ((*start) < end && line[(*start)] && line[(*start)] != '"')
	{
		if (line[(*start)] == '$')
		{
			tmp = secnd_expand_dollar(start, (line + (*start)), env);
			result = my_strjoin(result, tmp);
		}
		j = (*start);
		while ((*start) < end && line[(*start)] && line[(*start)] != '\"'
			&& line[(*start)] != '$')
			(*start)++;
		result = my_strjoin(result, ft_substr(line, j, (*start) - j));
	}
	(*start)++;
	return (result);
}

char	*grep_singleq(int end, int *start, const char *line)
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

char	*grep_no_quotes(t_token *tokens, int end, int *start, const char *line,
		t_env *env)
{
	char	*result;
	char	*tmp;
	int		j;

	(void)tokens;
	j = 0;
	result = NULL;
	tmp = NULL;
	if (line[(*start)] == '$')
	{
		tmp = secnd_expand_dollar(start, (line + (*start)), env);
		result = my_strjoin(result, tmp);
	}
	j = (*start);
	while ((*start) < end && line[(*start)] && line[(*start)] != '\"'
		&& line[(*start)] != '\'' && line[(*start)] != '$')
		(*start)++;
	result = my_strjoin(result, ft_substr(line, j, (*start) - j));
	return (result);
}
