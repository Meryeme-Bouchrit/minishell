/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:13:52 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/21 02:52:00 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

typedef enum e_token_type
{
	SPLIT,
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
	bool			expand;
	struct s_token	*next;
}					t_token;

typedef struct s_ctx
{
	int				end;
	int				start;
	const char		*line;
}					t_ctx;

//////////////////////token_list.c///////////////////////////////////////////
int					token_list_size(t_token *list);
char				**token_list_to_array(t_token *list);
void				free_token_list(t_token *list);
t_cmd				*new_cmd(void);
void				add_redirection(t_cmd *cmd, t_redirections type,
						const char *filename, bool expand);
////////////////////correct_token.c///////////////////////////////////////////
t_token				*tokenize(const char *line, t_env *env);

////////////////////////libft/////////////////////////////////////////////////
int					my_strcmp(const char *s1, const char *s2);
char				*ft_strncpy(char *dest, const char *src, size_t n);

/////////////////////utils_1.c/////////////////////////////////////////////////
int					ft_isspace(char c);
int					is_special(char c);
t_token				*new_token(char *value, t_token_type type);
void				add_token(t_token **head, char *val, t_token_type type);

//////////////////////parser.c//////////////////////////////////////////////////
t_cmd				*parse_commands(t_token *token);
void				print_commands(t_cmd *cmds);
//////////////////////continue_heredoc.c////////////////////////////////////////
char				*expand_variables(const char *line, t_env *env);
/////////////////////get_type.c/////////////////////////////////////////////////
t_token_type		get_type(char *s, int a);
bool				redirection(t_token_type type);
bool				is_redirection(char *s);
t_redirections		get_redirect_type(char *s);
/////////////////////free_cmds.c////////////////////////////////////////////////
void				free_cmds(t_cmd **cmds);
////////////////////my_strjoin.c////////////////////////////////////////////////
char				*my_strjoin(char *s1, char *s2);
int					check_for_syntax_error(t_token *token);
void				is_heredoc(t_token *tokens, const char *line, t_env *env);
////////////////expansion.c/////////////////////////////////////////////////////
int					empty_check(char *s);
char				*secnd_expand_dollar(t_ctx *ctx, int end, const char *str,
						t_env *env);
////////////////continue.c//////////////////////////////////////////////////////
int					end_len(const char *line, int i, bool heredoc);
char				*grep_doubleq(int *i, t_ctx *ctx, t_env *env);
char				*grep_singleq(int *i, t_ctx *ctx);
bool				check_for_espace(char *res);
char				*grep_no_quotes(int *i, t_ctx *ctx, t_env *env);

#endif
