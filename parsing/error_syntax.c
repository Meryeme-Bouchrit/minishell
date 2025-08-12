/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 04:53:27 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/08 04:53:31 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_for_syntax_error(t_token *token)
{
	while (token->next)
	{
		if (token->type != WORD && (token->type == token->next->type))
		{
			write(2, "Error! syntax error near unexpected token `newline'\n",
				53);
			// exit_status = 127;
			return (1);
		}
		token = token->next;
	}
	if (token->type != WORD)
	{
		write(2, "Error! syntax error near unexpected token `newline'\n", 53);
		return (1);
	}
	return (0);
}
