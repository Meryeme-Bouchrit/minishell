/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 09:32:14 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/04 23:16:09 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h" // your libft functions
# include <fcntl.h>       // for open flags (O_RDONLY, O_WRONLY, etc)
# include <stdbool.h>     // for bool, true, false
# include <stdio.h>       // for perror, printf (for debug)
# include <stdlib.h>
# include <errno.h>
# include <string.h>    // for strerror
# include <sys/types.h> // for pid_t
# include <sys/wait.h>  // for wait, waitpid

// Redirection type used to distinguish <, >, >>, <<
typedef enum e_redirections
{
	REDIR_IN,     // <
	REDIR_OUT,    // >
	REDIR_APPEND, // >>
	REDIR_HEREDOC // <<
}						t_redirections;

// Represents a redirection instruction (e.g., < infile, >> outfile)
typedef struct s_in_out_fds
{
	t_redirections		type;
	int					fd;
	char				*filename;
	char				*heredoc_delimiter;
	bool				heredoc_quotes;
	struct s_in_out_fds	*next;
}						t_in_out_fds;

// Stores execution-specific metadata (PID, path, status).
typedef struct s_pipex
{
	int					fork_pid;
	int					status;
	char				*path;
}						t_pipex;

// Represents a single command in the pipeline with redirections and args.
typedef struct s_cmd
{
	char				*command;
	char				**args;
	bool				pipe_output;
	t_in_out_fds		*io_fds;
	t_pipex				*pipex;
	struct s_cmd		*next;
	struct s_cmd		*prev;

	// Fields used by the executor:
	bool				is_builtin;
	int					fd_in;
	int					fd_out;
}						t_cmd;

/* ============================ execution ============================ */

/*  Builtin Function Prototypes  */

int		ft_cd(char **args, char **env);             // change directory
int		ft_pwd(void);                               // print working directory
int		ft_env(char **env);                         // print environment
int		ft_echo(char **args);                       // echo arguments
int		ft_exit(char **args, int *exit_status);     // exit shell
int		ft_unset(char **args, char **env);          // unset variables
int		ft_export(char **args, char ***env);        // export variables

/*  Env Management  */

char	**copy_env(char **envp);                             // duplicate environment
int		update_env_var(char **env, char *key, char *value);   // update key=value
char	**realloc_env(char **env, char *new_var);            // reallocate env
void	unset_env_var(char **env, char *key);                // remove key from env
int		env_has_key(char **env, char *key);                  // check if key exists

/* Export Helpers */

void	add_env(char ***env, char *key, char *value);        // add key=value
void	replace_env(char ***env, char *key, char *value);    // replace or add key=value
void	handle_env_update(char ***env, char *key, char *value); // smart update
void	parse_export_arg(char *arg, char **key, char **value); // split key=value
void	print_export_env(char **env);                        // print as declare -x
int		is_valid_key(char *key);                              // key format validation
char	*create_env_var(const char *key, const char *value); // join key=value

/*  Utility Functions  */

int		ft_strcmp(const char *s1, const char *s2);             // compare strings
void	free_split(char **split);                            // free array of strings
char	*ft_remove_quotes(char *str);                        // strip surrounding quotes
int		ft_is_numeric(const char *str);                        // check if numeric
void	print_echo_args(char **args, int i);                 // echo helper


// int		ft_execute_cmd(char **argv, char **envp);


/* ============================ parsing ============================ */





#endif