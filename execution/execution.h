/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:14:48 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 20:00:07 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
# include "minishell.h"
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdbool.h>

/* -------------------- Child Process -------------------- */
void	child_run(t_cmd *cmd, t_env *env);
void	child_single_run(t_cmd *cmd, char *path, t_env *env);
int		fork_one(t_cmd *cmd, t_env *env, pid_t *pids, int i);
int		fork_all(t_cmd *cmds, t_env *env, pid_t *pids);
void	exec_run(t_cmd *cmd, t_env *env);

/* -------------------- Environment -------------------- */
char	**env_to_envp(t_env *env);

/* -------------------- Execution -------------------- */
int		exec_cmd(t_cmd *cmd, t_env *env, int *status);
int		fork_and_wait(t_cmd *cmd, char *path, t_env *env);
int		exec_resolve(t_cmd *cmd, t_env *env, int *status);
int		status_to_exit(int status);

/* -------------------- Command Path -------------------- */
char	*get_path_envp(char **envp);
char	*find_bin(char *cmd, char **paths);
char	*find_cmd_path(char *cmd, char **envp);
int		check_cmd_errors(char *cmd, int *status);

/* -------------------- Pipelines -------------------- */
int		pipeline_run(t_cmd *cmds, t_env *env);
int		pipeline_end(pid_t *pids, int **pipes, int n, int ret);
int		**init_pipes(int n);
int		cmd_count(t_cmd *cmds);
void	close_all_pipes(int **pipes, int n);
void	setup_child(int **pipes, int idx, int total);

/* -------------------- Redirections -------------------- */
int		redir_file(char *file, int target_fd, int flags);
void	apply_redir(t_in_out_fds *redir);
void	apply_all_redirs(t_in_out_fds *redir);

/* -------------------- Heredoc -------------------- */
int		write_heredoc(int fd, char *line, t_env *env, bool expand);
int		heredoc_input(int fd, char *limiter, t_env *env, bool expand);
char	*heredoc_name(void);
int		heredoc_fd_new(char **path);
int		fork_heredoc(int fd, char *limiter, t_env *env, bool expand);
char	*wait_heredoc(pid_t pid, char *path);
char	*make_heredoc(char *limiter, t_env *env, bool expand);
int		handle_heredocs(t_cmd *cmd, t_env *env);
int		handle_all_heredocs(t_cmd *cmds, t_env *env);
void	heredocs_cleanup(t_cmd *cmds);

/* -------------------- Signals -------------------- */
void	sigint_prompt(int sig);
void	sigint_heredoc(int sig);

t_env	*dup_env(char **envp);
int		create_all_pipes(int **pipes, int n);
pid_t	*init_pids(int n, int **pipes);
int		wait_last(pid_t *pids, int n);

int		env_len(t_env *env);
char	*build_env_str(char *key, char *value);

/* ========================= MAIN HELPERS ========================= */
void	free_in_out_fds(t_in_out_fds *redir);
void	free_all(t_cmd *cmds, t_env *env);
void	handle_exit(t_cmd *cmds, t_env *env);
void	check_if_cmd(t_cmd *cmd, t_env **env);

#endif