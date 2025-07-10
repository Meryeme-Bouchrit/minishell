#include "mini.h"

char *extract_quoted(const char *str, int *i, char quote)
{
    int start = *i;
    (*i)++;

    while (str[*i] && str[*i] != quote)
        (*i)++;

    int len = *i - start - 1;
    char *result = malloc(len + 1);
    strncpy(result, str + start + 1, len);
    result[len] = '\0';

    if (str[*i] == quote)
        (*i)++;

    return result;
}
