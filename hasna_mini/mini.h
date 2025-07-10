#ifndef MINI_H
#define MINI_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
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

///////////////////////////cmd////////////////////

typedef enum e_redirections
{
    T_REDIR_IN,
    T_REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} t_redirections;

typedef struct s_in_out_fds
{
    t_redirections    type;
    char             *filename;
    struct s_in_out_fds *next;
} t_in_out_fds;

typedef struct s_cmd
{
    char          **args;     // Array of arguments
    t_in_out_fds  *io_fds;    // Redirection list
    bool           pipe_output;
    struct s_cmd  *next;
} t_cmd;

//////////////////////helper_func_1.c///////////////////////////////
char *extract_quoted(const char *str, int *i, char quote);

////////////////////helper_func_2.c//////////////////
void print_commands(t_cmd *cmds);
t_cmd *parse_commands(char **tokens);
int token_list_size(t_token *list);
char **token_list_to_array(t_token *list);
//void add_token(t_token **list, const char *val);
//t_token *new_token(const char *val);


#endif
