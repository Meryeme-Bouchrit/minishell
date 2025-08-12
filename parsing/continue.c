/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continue.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:48:26 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/08 04:36:48 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	end_len(const char *line, int i, bool heredoc)
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
			if ((line[end] == '\0') && !heredoc)
			{
				write(2, "ERROR! quotes are not closed!\n", 31);
				return (0);
			}
			end++;
		}
		else
			end++;
	}
	return (end);
}

char	*grep_doubleq(int end, int *start, const char *line, t_env *env)
{
	char	*tmp;
	char	*result;
	int		j;

	tmp = NULL;
	result = NULL;
	(*start)++;
	while ((*start) < end && line[(*start)] && line[(*start)] != '"')
	{
		if ((line[(*start)] == '$') && !env->heredoc)
		{
			tmp = secnd_expand_dollar(start, end - 1, (line + (*start)), env);
			result = my_strjoin(result, tmp);
		}
		j = (*start);
		while ((*start) < end && line[(*start)] && line[(*start)] != '\"'
			&& ((line[(*start)] != '$') || env->heredoc))
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

bool	check_for_espace(char *res)
{
	int	i;

	i = 0;
	while (res[i])
	{
		if (ft_isspace(res[i]))
			return (1);
		i++;
	}
	return (0);
}

char	*grep_no_quotes(int *i, int end, int *start, const char *line,
		t_env *env)
{
	char	*result;
	char	*tmp;
	int		j;

	j = 0;
	result = NULL;
	tmp = NULL;
	if ((line[(*start)] == '$') && !env->heredoc)
	{
		tmp = secnd_expand_dollar(start, end, (line + (*start)), env);
		result = my_strjoin(result, tmp);
		if (check_for_espace(result))
			i[1] = 1;
	}
	j = (*start);
	while ((*start) < end && line[(*start)] && line[(*start)] != '\"'
		&& line[(*start)] != '\'' && ((line[(*start)] != '$') || env->heredoc))
		(*start)++;
	result = my_strjoin(result, ft_substr(line, j, (*start) - j));
	return (result);
}
