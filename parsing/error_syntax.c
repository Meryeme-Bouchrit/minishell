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
