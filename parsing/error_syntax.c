/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 04:53:27 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/11 02:58:05 by mbouchri         ###   ########.fr       */
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
