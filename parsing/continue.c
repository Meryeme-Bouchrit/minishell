/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continue.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:48:26 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/21 02:48:30 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	end_len(const char *line, int i, bool heredoc)
{
	int		end;
	char	quote;

	end = i;
	while (line[end] && !ft_isspace(line[end]) && !is_special(line[end]))
	{
		if (line[end] == '#' && ((!line[end - 1]) || (line[end - 1] == ' ' )))
			break ;
		if (line[end] == '\"' || line[end] == '\'')
		{
			quote = line[end++];
			while (line[end] && line[end] != quote)
				end++;
			if ((line[end] == '\0') && !heredoc)
				return (write(2, "ERROR! quotes are not closed!\n", 31), 0);
			else if ((line[end] == '\0') && heredoc)
				return (end);
			end++;
		}
		else
			end++;
	}
	return (end);
}

char	*grep_doubleq(int *i, t_ctx *ctx, t_env *env)
{
	char	*tmp;
	char	*result;
	int		j;

	tmp = NULL;
	result = NULL;
	ctx->start++;
	while (ctx->start < ctx->end && ctx->line[ctx->start]
		&& ctx->line[ctx->start] != '"')
	{
		if ((ctx->line[ctx->start] == '$') && !env->heredoc)
		{
			tmp = secnd_expand_dollar(ctx, ctx->end - 1, (ctx->line
						+ ctx->start), env);
			result = my_strjoin(result, tmp);
		}
		j = ctx->start;
		while (ctx->start < ctx->end && ctx->line[ctx->start]
			&& ctx->line[ctx->start] != '\"' && ((ctx->line[ctx->start] != '$')
				|| env->heredoc))
			ctx->start++;
		result = my_strjoin(result, ft_substr(ctx->line, j, ctx->start - j));
	}
	i[1] = 4;
	return (++ctx->start, result);
}

char	*grep_singleq(int *i, t_ctx *ctx)
{
	int		j;
	char	*result;

	j = 0;
	result = NULL;
	ctx->start++;
	j = ctx->start;
	while (ctx->start < ctx->end && ctx->line[ctx->start] != '\'')
		ctx->start++;
	result = my_strjoin(result, ft_substr(ctx->line, j, ctx->start - j));
	ctx->start++;
	i[1] = 4;
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

char	*grep_no_quotes(int *i, t_ctx *ctx, t_env *env)
{
	char	*result;
	char	*tmp;
	int		j;

	j = 0;
	result = NULL;
	tmp = NULL;
	if ((ctx->line[ctx->start] == '$') && !env->heredoc)
	{
		tmp = secnd_expand_dollar(ctx, ctx->end, (ctx->line + ctx->start), env);
		result = my_strjoin(result, tmp);
		if (check_for_espace(result))
			i[1] = 1;
	}
	j = ctx->start;
	while (ctx->start < ctx->end && ctx->line[ctx->start]
		&& ctx->line[ctx->start] != '\"' && ctx->line[ctx->start] != '\''
		&& ((ctx->line[ctx->start] != '$') || env->heredoc))
		ctx->start++;
	result = my_strjoin(result, ft_substr(ctx->line, j, ctx->start - j));
	return (result);
}
