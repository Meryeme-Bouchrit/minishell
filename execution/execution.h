/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 05:57:10 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/27 10:04:18 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <ctype.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include "minishell.h" // ✅ Shared structs

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* ========== EXECUTION ========== */

int		ft_execute_cmd(t_cmd *cmd, char **envp);
void	ft_child_process(t_cmd *cmd, char *cmd_path, char **envp);
char	*ft_get_command_path(char *cmd, char **envp);
char	*get_path(char **envp);
char	*find_bin(char *cmd, char **paths);

/* ====== Added prototypes needed by main.c ====== */
int		only_spaces(const char *str);

int		is_builtin(char *cmd);
int		run_builtin(char **args, t_env **env, int *exit_status);
int		exec_builtin_with_redir(t_cmd *cmd, t_env **env, int *exit_status);

/* ========== REDIRECTION ========== */

void	redir_in(char *filename);
void	redir_out(char *filename);
void	redir_app(char *filename);
void	redir_heredoc(char *limiter);

/* ========== PIPE ========== */

int		ft_execute_pipeline(t_cmd *cmd, char **envp);

/* ========== BUILTINS ========== */

int		ft_cd(char **args, t_env **env);
int		ft_pwd(void);
int		ft_env(t_env *env);
int		ft_echo(char **args);
int		ft_exit(char **args, int *exit_status);
int		ft_unset(char **args, t_env **env);
int		ft_export(char **args, t_env **env);

/* ========== ENV UTILS ========== */

t_env	*copy_env(char **envp);
void	free_env_list(t_env *env);
void	add_env(t_env **env, char *key, char *value);
void	unset_env_var(t_env **env, char *key);
t_env	*find_env_node(t_env *env, const char *key);
int		env_has_key(t_env *env, char *key);
char	*get_env_value(t_env *env, const char *key);
void	replace_env(t_env **env, char *key, char *value);
void	handle_env_update(t_env **env, char *key, char *value);
void	print_export_env(t_env *env);
int		is_valid_key(char *key);
char	*create_env_var(const char *key, const char *value);
void	add_node_back(t_env **env, t_env *new);
void	set_env_var(t_env **env, const char *key, const char *value);
void	parse_export_arg(char *arg, char **key, char **value);

/* ========== UTILS ========== */

int		ft_strcmp(const char *s1, const char *s2);
void	free_split(char **split);
char	*ft_remove_quotes(char *str);
int		ft_is_numeric(const char *str);
void	print_echo_args(char **args, int i);

#endif
