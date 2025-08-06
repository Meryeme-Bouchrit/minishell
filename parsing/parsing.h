/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:24:44 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/06 03:02:55 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
# include "minishell.h" // ✅ Shared structs
# include <ctype.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	bool			freed;
	struct s_token	*next;
}					t_token;

//////////////////////helper_func_1.c///////////////////////////////
char				*extract_quoted(const char *str, int *i, char quote);

////////////////////helper_func_2.c//////////////////
void				print_commands(t_cmd *cmds);
int					token_list_size(t_token *list);
char				**token_list_to_array(t_token *list);
void				add_token(t_token **head, char *val, t_token_type type);

bool				is_redirection(char *s);
t_redirections		get_redirect_type(char *s);

//////////////////////helper_func_2_1.c////////////////////////////
void				add_redirection(t_cmd *cmd, t_redirections type,
						const char *filename);
t_cmd				*new_cmd(void);
void				free_token_list(t_token *list);
char				**token_list_to_array(t_token *list);
int					token_list_size(t_token *list);

////////////////////helper_func_3.c///////////////
void				add_token(t_token **head, char *val, t_token_type type);
t_token_type		get_type(char *s);
t_token				*new_token(char *value, t_token_type type);
int					is_special(char c);

////////////////////////libft////////////////////////////////
int					my_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strdup(const char *s1);
char				*ft_strncpy(char *dest, const char *src, size_t n);

/////////////////////utils_1.c///////////////////
int					ft_isspace(char c);
t_cmd				*parse_commands(t_token *token);
void				secnd_free_token_list(t_token **list);

/* ===== Added prototypes needed by main.c ===== */
// t_token				*tokenize(const char *line);
void				free_cmds(t_cmd **cmds);

char				*expand_if_needed(char *token_value, char quote_type);
char				*expand_dollar(const char *str);
t_token				*tokenize(const char *line, t_env *env);
char				*grep_doubleq(t_token *tokens, int end, int *start,
						const char *line, t_env *env);
char				*grep_singleq(int end, int *start, const char *line);
char				*grep_no_quotes(t_token *tokens, int end, int *start,
						const char *line, t_env *env);
int					end_len(const char *line, int i);

char				*my_strjoin(char *s1, char *s2);
char				*secnd_expand_dollar(int *start, const char *str,
						t_env *env);

int					check_for_syntax_error(t_token *token);

#endif
