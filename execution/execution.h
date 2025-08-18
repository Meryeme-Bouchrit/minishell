/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 05:57:10 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/16 13:45:00 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

/* ===================== STANDARD LIBRARIES ===================== */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

/* ===================== READLINE ===================== */
# include <readline/readline.h>
# include <readline/history.h>

/* ===================== PROJECT HEADERS ===================== */
# include "../libft/libft.h"
# include "minishell.h"

# ifndef O_DIRECTORY
#  define O_DIRECTORY 0
# endif

/* ===================== EXECUTION CORE ======================== */

/* exec_cmd.c */
int     exec_cmd(t_cmd *cmd, t_env *env, int *status);
void    child_exec(t_cmd *cmd, char *path, t_env *env);

/* exec_pipe.c */
int     exec_pipeline(t_cmd *cmds, t_env *env);

/* exec_env.c */
char    **env_to_envp(t_env *env);

/* exec_path.c */
char    *find_cmd_path(char *cmd, char **envp);
char    *get_path_envp(char **envp);
char    *get_path_env(t_env *env);
char    *find_bin(char *cmd, char **paths);

/* exec_redirs.c */
int     redir_in(char *filename);
int     redir_out(char *filename);
int     redir_app(char *filename);
void    ft_handle_redirs(t_in_out_fds *redir);
char    *redir_heredoc(char *limiter, t_env *env, bool expand);
int     ft_preprocess_heredocs(t_cmd *cmds, t_env *env);

/* ======================= BUILTINS ============================ */

int     is_builtin(char *cmd);
int     run_builtin(char **args, t_env **env, int *status);
int     ft_cd(char **args, t_env **env);
int     ft_pwd(void);
int     ft_env(t_env *env);
int     ft_echo(char **args);
int     ft_exit(char **args, int *status);
int     ft_unset(char **args, t_env **env);
int     ft_export(char **args, t_env **env);

/* ==================== ENVIRONMENT UTILS ====================== */

t_env   *dup_env(char **envp);
void    free_env_list(t_env *env);
void    add_env(t_env **env, char *key, char *value);
void    unset_env_var(t_env **env, char *key);
t_env   *find_env_node(t_env *env, const char *key);
int     env_has_key(t_env *env, char *key);
char    *env_get(t_env *env, const char *key);
void    env_replace(t_env **env, char *key, char *value);
void    env_update(t_env **env, char *key, char *value);
void    print_export_env(t_env *env);

int     is_valid_key(char *key);
char    *create_env_var(const char *key, const char *value);
void    add_node_back(t_env **env, t_env *new);
void    env_set(t_env **env, const char *key, const char *value);
void    parse_export_arg(char *arg, char **key, char **value);

t_env   *env_new(char *key, char *value);
t_env   *new_env(char *envp_entry, t_env **env);

/* ======================== SIGNALS ============================ */

void    sigquit_prompt(int sig);
void    sigint_prompt(int sig);
void    sigint_heredoc(int sig);

/* ========================= UTILS ============================= */

int     ft_strcmp(const char *s1, const char *s2);
int     ft_is_numeric(const char *str);
void    free_split(char **split);
char    *ft_remove_quotes(char *str);
int     only_spaces(const char *str);

int     heredoc_child_loop(int out_fd, char *limiter, t_env *env, bool expand);
char    *generate_temp_filename(void);
int     redir_in_fd(int fd);
int     fork_and_wait(t_cmd *cmd, char *path, t_env *env);
// void    handle_exit(t_cmd *cmds, t_env **env);
void cleanup_heredocs(t_cmd *cmds);

#endif
