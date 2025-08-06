#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*remove_quotes(const char *str)
{
	size_t	len;

	len = strlen(str);
	if (len >= 2 && str[0] == '\"' && str[len - 1] == '\"')
	{
		char *new_str = malloc(len - 1); // len - 2 for quotes, +1 for '\0'
		if (!new_str)
			return (NULL);
		strncpy(new_str, str + 1, len - 2);
		new_str[len - 2] = '\0';
		return (new_str);
	}
	return (strdup(str)); // return a copy as-is if no surrounding quotes
}

int	main(void)
{
	const char	*original = "\"hello\"";
	char		*cleaned;

	cleaned = remove_quotes(original);
	printf("%s\n", cleaned); // Output: hello
	free(cleaned);
	return (0);
}
