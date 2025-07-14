/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:50:36 by zhassna           #+#    #+#             */
/*   Updated: 2025/07/14 06:45:15 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_token	*tokenize(const char *line)
{
	int		i;
	int		start;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		while (isspace(line[i]))
			i++;
		if (is_special(line[i]))
		{
			if (line[i] == line[i + 1]) // >> or <<
			{
				add_token(&tokens, ft_substr(line, i, 2), get_type(ft_substr(line, i,
							2)));
				i += 2;
			}
			else
			{
				add_token(&tokens, ft_substr(line, i, 1), get_type(ft_substr(line, i,
							1)));
				i++;
			}
		}
		else if (line[i] == '\'' || line[i] == '"')
			add_token(&tokens, extract_quoted(line, &i, line[i]), WORD);
		else if (line[i])
		{
			start = i;
			while (line[i] && !isspace(line[i]) && !is_special(line[i])
				&& line[i] != '\'' && line[i] != '"')
				i++;
			add_token(&tokens, ft_substr(line, start, i - start), WORD);
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

int	main(void)
{
	char	*line;
	t_token	*tokens;
	char	**array;
	t_cmd	*cmds;

	while (1)
	{
		line = readline("🐚 minishell$ ");
		if (!line)
		{
			printf("Goodbye!\n");
			break ;
		}
		tokens = tokenize(line);
		//print_tokens(tokens);
		//  char *tokensi[] = {"echo", "hello world", ">", "out.txt", "|", "cat", "-n", NULL};
		array = token_list_to_array(tokens);
		cmds = parse_commands(array);
		print_commands(cmds);
		if (*line)
			add_history(line);
		free(line);
		free_token_list(tokens);
	}
	return (0);
}
