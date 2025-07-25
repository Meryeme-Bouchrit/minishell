#include "mini.h"

#include <stdlib.h>

char	*expand_dollar(const char *str)
{
	int		i = 0;
	char	var[256];
	char	*value;
	char	*result;
	int		vlen = 0;

	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		var[vlen++] = str[i++];
	var[vlen] = '\0';

	value = getenv(var); // get variable from env
	if (!value)
		value = ""; // if var doesn't exist

	// join value + rest of string after var
	result = ft_strjoin(value, str + i);
	return (result);
}

char	*expand_if_needed(char *token_value, char quote_type)
{
	if (quote_type == '\'') // single quotes? don't expand
		return (ft_strdup(token_value));

	char	*result = NULL;
	char	*ptr = token_value;
	char	*tmp;

	result = ft_calloc(1, sizeof(char));
	while (*ptr)
	{
		if (*ptr == '$' && *(ptr + 1) && (ft_isalpha(*(ptr + 1)) || *(ptr + 1) == '_'))
		{
			ptr++;
			tmp = expand_dollar(ptr);
			result = ft_strjoin_free(result, tmp); // You can define this helper to join + free
			while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
				ptr++;
		}
		else
		{
			char s[2] = {*ptr, 0};
			tmp = ft_strjoin(result, s);
			free(result);
			result = tmp;
			ptr++;
		}
	}
	return (result);
}

void    shell_expansion()
{
    if (double_quoted or no quotes)
    {
        expand();
    }
    else
    {
        dont_expand();
    }
}
