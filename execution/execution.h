/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 05:57:10 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/19 14:00:00 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

/* ===================== STANDARD LIBRARIES ===================== */
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/* ===================== READLINE ===================== */
# include <readline/history.h>
# include <readline/readline.h>

/* ===================== PROJECT HEADERS ===================== */
# include "../libft/libft.h"
# include "minishell.h"

# ifndef O_DIRECTORY
#  define O_DIRECTORY 0
# endif

/* ===================== EXEC PIPE CORE ======================== */

int		exec_pipeline(t_cmd *cmds, t_env *env);
int		end_pipeline(pid_t *pids, int **pipes, int n, int ret);
int		**init_pipes(int n);
pid_t	*init_pids(int n, int **pipes);
int		fork_one(t_cmd *cmd, t_env *env, pid_t *pids, int i);
int		fork_all(t_cmd *cmds, t_env *env, pid_t *pids);
int		wait_last(pid_t *pids, int n);
void	close_all_pipes(int **pipes, int n);
void	setup_child(int **pipes, int idx, int total);
void	print_exit(char *cmd, char *msg, int code);
void	check_cmd_errors(t_cmd *cmd);
void	child_process(t_cmd *cmd, t_env *env);
void	run_cmd(t_cmd *cmd, t_env *env);
int		count_cmds(t_cmd *cmds);
int		create_all_pipes(int **pipes, int n);

/* ===================== EXEC CMD ======================== */
int		exec_cmd(t_cmd *cmd, t_env *env, int *status);

/* ===================== EXEC ENV ======================== */
char	**env_to_envp(t_env *env);

/* ===================== EXEC PATH ======================= */
char	*find_cmd_path(char *cmd, char **envp);
char	*get_path_envp(char **envp);
char	*get_path_env(t_env *env);
char	*find_bin(char *cmd, char **paths);

/* ===================== EXEC REDIRS ===================== */
int		redirect_file(char *file, int target_fd, int flags);
void	apply_redir(t_in_out_fds *redir);
void	apply_all_redirs(t_in_out_fds *redir);

int		write_heredoc(int fd, char *line, t_env *env, bool expand);
char	*read_line(void);
int		heredoc_input(int fd, char *limiter, t_env *env, bool expand);

char	*new_heredoc_name(void);
int		create_heredoc_fd(char **path);
int		fork_heredoc(int fd, char *limiter, t_env *env, bool expand);
char	*wait_heredoc(pid_t pid, char *path);
char	*make_heredoc(char *limiter, t_env *env, bool expand);

void	remove_cmd_heredocs(t_cmd *cmd);
int		handle_heredocs(t_cmd *cmd, t_env *env);
int		handle_all_heredocs(t_cmd *cmds, t_env *env);
void	cleanup_all_heredocs(t_cmd *cmds);

/* ======================= BUILTINS ====================== */
int		is_builtin(char *cmd);
int		run_builtin(char **args, t_env **env, int *status);
int		ft_cd(char **args, t_env **env);
int		ft_pwd(void);
int		ft_env(t_env *env);
int		ft_echo(char **args);
int		ft_exit(char **args, int *status);
int		ft_unset(char **args, t_env **env);
int		ft_export(char **args, t_env **env);

/* ==================== ENVIRONMENT UTILS ====================== */
t_env	*dup_env(char **envp);
void	free_env_list(t_env *env);
void	add_env(t_env **env, char *key, char *value);
void	unset_env_var(t_env **env, char *key);
t_env	*find_env_node(t_env *env, const char *key);
int		env_has_key(t_env *env, char *key);
char	*env_get(t_env *env, const char *key);
void	env_replace(t_env **env, char *key, char *value);
void	env_update(t_env **env, char *key, char *value);
void	print_export_env(t_env *env);

int		is_valid_key(char *key);
char	*create_env_var(const char *key, const char *value);
void	add_node_back(t_env **env, t_env *new);
void	env_set(t_env **env, const char *key, const char *value);
void	parse_export_arg(char *arg, char **key, char **value);

t_env	*env_new(char *key, char *value);
t_env	*new_env(char *envp_entry, t_env **env);

/* ======================== SIGNALS ============================ */
void	sigquit_prompt(int sig);
void	sigint_prompt(int sig);
void	sigint_heredoc(int sig);

/* ========================= UTILS ============================= */
int		ft_strcmp(const char *s1, const char *s2);
int		ft_is_numeric(const char *str);
void	free_split(char **split);
char	*ft_remove_quotes(char *str);
int		only_spaces(const char *str);

int		fork_and_wait(t_cmd *cmd, char *path, t_env *env);
int		check_directory(char *cmd, int *status);
int		check_access_errors(char *cmd, int *status);
int		resolve_and_exec(t_cmd *cmd, t_env *env, int *status);

#endif
