/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:50:36 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/18 00:24:04 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("TOKEN: %-10s TYPE: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

// void	free_cmds(t_cmd **cmds, int i)
// {
// 	t_in_out_fds	*redir;
// 	t_in_out_fds *tmp_redir;
// 	t_cmd			*tmp;

// 	while (*cmds)
// 	{
// 		if ((*cmds)->args)
// 		{
// 			while ((*cmds)->args[i])
// 				free((*cmds)->args[i++]);
// 			free((*cmds)->args);
// 		}
// 		if ((*cmds)->io_fds)
// 		{
// 			redir = (*cmds)->io_fds;
// 			while (redir)
// 			{
// 				tmp_redir = redir;
// 				free(redir->filename);
// 				redir = redir->next;
// 				free(tmp_redir);
// 			}
// 			//free((*cmds)->io_fds);
// 		}
// 		tmp = (*cmds);
// 		(*cmds) = (*cmds)->next;
// 		free(tmp);
// 	}
// }

void	free_cmds(t_cmd **cmds)
{
	t_in_out_fds	*redir;
	t_cmd			*tmp;
	int				i;

	while (*cmds)
	{
		i = 0;
		while ((*cmds)->args && (*cmds)->args[i])
			free((*cmds)->args[i++]);
		free((*cmds)->args);
		while ((*cmds)->io_fds)
		{
			redir = (*cmds)->io_fds;
			(*cmds)->io_fds = redir->next;
			free(redir->filename);
			free(redir);
		}
		tmp = *cmds;
		*cmds = (*cmds)->next;
		free(tmp);
	}
}
