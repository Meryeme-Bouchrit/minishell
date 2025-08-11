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
		token = token->next;
	if (token->type != WORD)
	{
		printf("Error! syntax error near unexpected token `newline'\n");
		return (1);
	}
	return (0);
}
