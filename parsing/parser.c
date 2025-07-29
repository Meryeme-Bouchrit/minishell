/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:50:36 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/27 10:13:23 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*tokenize(const char *line)
{
	int		i;
	int		start;
	t_token	*tokens;
	char	*quoted_str;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (is_special(line[i]))
		{
			if (line[i] == line[i + 1])
			{
				add_token(&tokens, ft_substr(line, i, 2),
					get_type(ft_substr(line, i, 2)));
				i++;
			}
			else
				add_token(&tokens, ft_substr(line, i, 1),
					get_type(ft_substr(line, i, 1)));
			i++;
		}
		else if (line[i] == '\'' || line[i] == '"')
		{
			quoted_str = extract_quoted(line, &i, line[i]);
			if (quoted_str && quoted_str[0] != '\0')
			{
			//	add_token(&tokens, quoted_str, WORD);
				char *expanded = expand_if_needed(quoted_str, line[i]);
				add_token(&tokens, expanded, WORD);
				free(quoted_str);

			}
			else
				free(quoted_str);
		}
		else if (line[i])
		{
			start = i;
			while (line[i] && !ft_isspace(line[i]) && !is_special(line[i])
				&& line[i] != '\'' && line[i] != '"')
				i++;
		//	add_token(&tokens, ft_substr(line, start, i - start), WORD);
			char *raw = ft_substr(line, start, i - start);
			char *expanded = expand_if_needed(raw, 0);
			add_token(&tokens, expanded, WORD);
			free(raw);
		}
	}
	return (tokens);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("TOKEN: %-10s TYPE: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

void	free_cmds(t_cmd **cmds)
{
	int				i;
	t_in_out_fds	*redir;
	t_cmd			*tmp;

	while (*cmds)
	{
		if ((*cmds)->args)
		{
			i = 0;
			while ((*cmds)->args[i])
				free((*cmds)->args[i++]);
			free((*cmds)->args);
		}
		if ((*cmds)->io_fds)
		{
			redir = (*cmds)->io_fds;
			while (redir)
			{
				free(redir->filename);
				redir = redir->next;
			}
			free((*cmds)->io_fds);
		}
		tmp = (*cmds);
		(*cmds) = (*cmds)->next;
		free(tmp);
	}
}

// int	main(void)
// {
// 	char	*line;
// 	t_token	*tokens;
// 	t_cmd	*cmds;

// 	while (1)
// 	{
// 		line = readline("🐚 minishell$ ");
// 		if (!line)
// 		{
// 			printf("Goodbye!\n");
// 			break ;
// 		}
// 		tokens = tokenize(line);
// 		cmds = parse_commands(tokens);
// 		print_commands(cmds);
// 		if (*line)
// 			add_history(line);
// 		free(line);
// 		free_token_list(tokens);
// 		free_cmds(&cmds);
// 	}
// 	return (0);
// }
