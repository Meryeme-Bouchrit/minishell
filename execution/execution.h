/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:14:48 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/22 12:29:47 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
# include "minishell.h"

# ifndef O_DIRECTORY
#  define O_DIRECTORY 0
# endif

int		wait_last(pid_t *pids, int n);
void	child_process(t_cmd *cmd, t_env *env);

char	**env_to_envp(t_env *env);
t_env	*dup_env(char **envp);
void	unset_env_var(t_env **env, char *key);

int		exec_cmd(t_cmd *cmd, t_env *env, int *status);
int		fork_and_wait(t_cmd *cmd, char *path, t_env *env);
int		check_directory(char *cmd, int *status);
int		check_access_errors(char *cmd, int *status);
int		resolve_and_exec(t_cmd *cmd, t_env *env, int *status);

void	child_process_single(t_cmd *cmd, char *path, t_env *env);
int		get_exit_status(int status);
int		fork_one(t_cmd *cmd, t_env *env, pid_t *pids, int i);
int		fork_all(t_cmd *cmds, t_env *env, pid_t *pids);
void	run_cmd(t_cmd *cmd, t_env *env);

char	*get_path_envp(char **envp);
char	*get_path_env(t_env *env);
char	*find_bin(char *cmd, char **paths);
char	*find_cmd_path(char *cmd, char **envp);

int		exec_pipeline(t_cmd *cmds, t_env *env);
int		end_pipeline(pid_t *pids, int **pipes, int n, int ret);
int		**init_pipes(int n);
pid_t	*init_pids(int n, int **pipes);
int		count_cmds(t_cmd *cmds);

int		create_all_pipes(int **pipes, int n);
void	close_all_pipes(int **pipes, int n);
void	setup_child(int **pipes, int idx, int total);
void	print_exit(char *cmd, char *msg, int code);
void	check_cmd_errors(t_cmd *cmd);

int		redirect_file(char *file, int target_fd, int flags);
void	apply_redir(t_in_out_fds *redir);
void	apply_all_redirs(t_in_out_fds *redir);
int		write_heredoc(int fd, char *line, t_env *env, bool expand);
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

void	sigint_prompt(int sig);
void	sigquit_prompt(int sig);
void	sigint_heredoc(int sig);

#endif
