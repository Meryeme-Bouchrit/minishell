#include "mini.h"

int is_special(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

t_token *new_token(char *value, t_token_type type)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token) return NULL;
    token->value = value;
    token->type = type;
    token->next = NULL;
    return token;
}

t_token_type get_type(char *s)
{
    if (strcmp(s, "|") == 0) return PIPE;
    if (strcmp(s, "<") == 0) return REDIR_IN;
    if (strcmp(s, ">") == 0) return REDIR_OUT;
    if (strcmp(s, ">>") == 0) return APPEND;
    if (strcmp(s, "<<") == 0) return HEREDOC;
    return WORD;
}

char *substr(const char *s, int start, int len)
{
    char *sub = malloc(len + 1);
    if (!sub) return NULL;
    for (int i = 0; i < len; i++)
        sub[i] = s[start + i];
    sub[len] = '\0';
    return sub;
}

void add_token(t_token **head, char *val, t_token_type type)
{
    t_token *new = new_token(val, type);
    if (!*head)
        *head = new;
    else {
        t_token *temp = *head;
        while (temp->next)
            temp = temp->next;
        temp->next = new;
    }
}

t_token *tokenize(const char *line)
{
    int i = 0, start;
    t_token *tokens = NULL;

    while (line[i])
    {
        while (isspace(line[i]))
            i++;

        if (is_special(line[i]))
        {
            if (line[i] == line[i+1]) // >> or <<
                add_token(&tokens, substr(line, i, 2), get_type(substr(line, i, 2))), i += 2;
            else
                add_token(&tokens, substr(line, i, 1), get_type(substr(line, i, 1))), i++;
        }
        else if (line[i])
        {
            start = i;
            while (line[i] && !isspace(line[i]) && !is_special(line[i]))
                i++;
            add_token(&tokens, substr(line, start, i - start), WORD);
        }
    }
    return tokens;
}


void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("TOKEN: %-10s TYPE: %d\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

int main()
{
	char *line;
	while (1)
	{
		line = readline("🐚 minishell$ ");
        	if (!line)
       		{
            		printf("Goodbye!\n");
            		break ;
        	}
		t_token *tokens = tokenize(line);
		print_tokens(tokens);
        	if (*line)
            		add_history(line);
   	//     printf("🧠 You said: %s\n", line);
        	free(line); // clean up!
    	}
	return (0);
}
