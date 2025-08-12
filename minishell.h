/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:34:42 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/12 06:23:18 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>

typedef enum e_redirections
{
	T_REDIR_IN,
	T_REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redirections;

typedef struct s_in_out_fds
{
	t_redirections		type;
	char				*filename;
	bool				expand;
	struct s_in_out_fds	*next;
}	t_in_out_fds;

typedef struct s_cmd
{
	char				**args;
	t_in_out_fds		*io_fds;
	bool				pipe_output;
	struct s_cmd		*next;
}	t_cmd;

typedef struct s_env
{
        char                    *key;
        char                    *value;
        bool			heredoc;
	struct s_env    *next;
}       t_env;

extern int exit_status;

# include "execution/execution.h"
# include "parsing/parsing.h"

#endif
