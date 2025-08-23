/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correct_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:05:47 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/24 00:00:09 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*token_next_string(t_token *tokens, t_ctx *ctx, int *i, t_env *env)
{
	char	*result;
	t_token	*tmp;

	ctx->start = (*i);
	tmp = tokens;
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	env->heredoc = false;
	is_heredoc(tmp, ctx->line, env);
	ctx->end = end_len(ctx->line, (*i), env->heredoc);
	if (ctx->end == 0)
		return (free_token_list(tokens), free(result), NULL);
	while (ctx->start < ctx->end)
	{
		if (ctx->line[ctx->start] == '\"')
			result = my_strjoin(result, grep_doubleq(i, ctx, env));
		else if (ctx->line[ctx->start] == '\'')
			result = my_strjoin(result, grep_singleq(i, ctx));
		else
			result = my_strjoin(result, grep_no_quotes(i, ctx, env));
	}
	env->heredoc = false;
	(*i) = ctx->start;
	return (result);
}

static char	*get_next_special_token(const char *line, int *i)
{
	if (is_special(line[i[0]]) && line[i[0]])
	{
		if (line[i[0] + 1] && ((line[i[0]] == '<' && line[i[0] + 1] == '<')
				|| (line[i[0]] == '>' && line[i[0] + 1] == '>')))
		{
			i[0] += 2;
			return (ft_substr(line, i[0] - 2, 2));
		}
		else
		{
			i[0]++;
			return (ft_substr(line, i[0] - 1, 1));
		}
	}
	return (NULL);
}

int	adds_token(char *token_value, t_token **tokens, int *i)
{
	if (token_value[0] || (!token_value[0] && i[1] == 4))
	{
		add_token(tokens, ft_strdup(token_value), get_type(token_value, i[1]));
		if ((*tokens)->type == PIPE)
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 51);
			free_token_list(*tokens);
			g_exit = 2;
			free(token_value);
			return (0);
		}
	}
	free(token_value);
	return (1);
}

int	upper_parsing(const char *line, int *i)
{
	i[1] = 0;
	if ((line[i[0]] == '#') && ((!line[i[0] - 1]) || (line[i[0] - 1] == ' ')))
		return (0);
	while (ft_isspace(line[i[0]]))
		i[0]++;
	if (!line[i[0]])
		return (0);
	return (1);
}

t_token	*tokenize(const char *line, t_env *env)
{
	int		i[2];
	char	*token_value;
	t_token	*tokens;
	t_ctx	ctx;

	i[0] = 0;
	tokens = NULL;
	ctx.line = line;
	while (i[0] < (int)ft_strlen(line))
	{
		if (!upper_parsing(line, i))
			break ;
		if (is_special(line[i[0]]) && line[i[0]])
			token_value = get_next_special_token(line, i);
		else
		{
			i[1] = 2;
			token_value = token_next_string(tokens, &ctx, i, env);
		}
		if (!token_value || (!adds_token(token_value, &tokens, i)))
			return (NULL);
	}
	if (check_for_syntax_error(tokens))
		return (free_token_list(tokens), NULL);
	return (tokens);
}
