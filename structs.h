/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhassna <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:50:04 by zhassna           #+#    #+#             */
/*   Updated: 2025/08/21 11:52:59 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	bool				expand;
	struct s_in_out_fds	*next;
}						t_in_out_fds;

typedef struct s_cmd
{
	char				**args;
	t_in_out_fds		*io_fds;
	bool				pipe_output;
	struct s_cmd		*next;
	int					idx;
	int					total;
	int					**pipe_fds;
}						t_cmd;

typedef struct s_env
{
	char				*key;
	char				*value;
	bool				heredoc;
	struct s_env		*next;
}						t_env;
