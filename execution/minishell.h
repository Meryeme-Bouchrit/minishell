/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 09:32:14 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/14 14:55:20 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>

/* ================= ENVIRONMENT STRUCT ================= */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* ================= STRUCTS ================= */

typedef enum e_redirections
{
	T_REDIR_IN,
	T_REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redirections;

typedef struct s_in_out_fds
{
	t_redirections		type;
	char				*filename;
	struct s_in_out_fds	*next;
}						t_in_out_fds;

typedef struct s_cmd
{
	char				**args;
	t_in_out_fds		*io_fds;
	bool				pipe_output;
	struct s_cmd		*next;
}						t_cmd;

/* ================= BUILTIN PROTOTYPES ================= */

int		ft_cd(char **args, t_env **env);
int		ft_pwd(void);
int		ft_env(t_env *env);
int		ft_echo(char **args);
int		ft_exit(char **args, int *exit_status);
int		ft_unset(char **args, t_env **env);
int		ft_export(char **args, t_env **env);

/* ================= ENV MANAGEMENT ================= */

t_env	*copy_env(char **envp);
void	free_env_list(t_env *env);
void	add_env(t_env **env, char *key, char *value);
void	unset_env_var(t_env **env, char *key);
int		env_has_key(t_env *env, char *key);
t_env	*find_env_node(t_env *env, const char *key);
char	*get_env_value(t_env *env, const char *key);

/* ================= EXPORT HELPERS ================= */

void	add_env(t_env **env, char *key, char *value);
void	replace_env(t_env **env, char *key, char *value);
void	handle_env_update(t_env **env, char *key, char *value);
void	parse_export_arg(char *arg, char **key, char **value);
void	print_export_env(t_env *env);
int		is_valid_key(char *key);
char	*create_env_var(const char *key, const char *value);
void	add_node_back(t_env **env, t_env *new);
void	set_env_var(t_env **env, const char *key, const char *value);


/* ================= UTILS ================= */

int		ft_strcmp(const char *s1, const char *s2);
void	free_split(char **split);
char	*ft_remove_quotes(char *str);
int		ft_is_numeric(const char *str);
void	print_echo_args(char **args, int i);

/* ================= Execution ================= */

int		ft_execute_cmd(t_cmd *cmd, char **envp);
char 	*get_command_path(char *cmd, char **envp);


#endif
