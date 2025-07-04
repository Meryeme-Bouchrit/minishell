#ifndef MINI_H
#define MINI_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>



typedef enum e_token_type {
    WORD,
    PIPE,
    REDIR_IN,
    REDIR_OUT,
    APPEND,
    HEREDOC
} t_token_type;

typedef struct s_token {
    char *value;             // the word or symbol
    t_token_type type;       // type of token
    struct s_token *next;
} t_token;


#endif
