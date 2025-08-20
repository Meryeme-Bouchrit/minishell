/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:41:18 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/20 03:02:36 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_in_out_fds(t_in_out_fds *redir)
{
	t_in_out_fds	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->filename)
		{
			unlink(tmp->filename);
			free(tmp->filename);
		}
		free(tmp);
	}
}

void	free_all(t_cmd *cmds, t_env *env)
{
	if (cmds)
		free_cmds(&cmds);
	if (env)
		free_env_list(env);
	rl_clear_history();
}

void	handle_exit(t_cmd *cmds, t_env *env)
{
	int	ret;

	ret = ft_exit(cmds->args, &g_exit);
	if (ret == -1)
	{
		free_all(cmds, env);
		exit(g_exit);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	t_token	*tok;
	t_cmd	*cmd;
	char	*line;
	int		skip;
	int		run;
	int		skip_cmd;
	int		hasna;

	(void)argc;
	(void)argv;
	env = dup_env(envp);
	g_exit = 0;
	signal(SIGINT, sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
	hasna = 1;
	while (hasna)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		skip = only_spaces(line);
		skip_cmd = 0;
		if (!skip)
		{
			add_history(line);
			tok = tokenize(line, env);
			cmd = parse_commands(tok);
			free_token_list(tok);
			if (cmd)
			{
				if (handle_all_heredocs(cmd, env) != 0)
					skip_cmd = 1;
				if (!skip_cmd && cmd->args && cmd->args[0])
				{
					run = (!cmd->next && !cmd->io_fds);
					if (ft_strcmp(cmd->args[0], "exit") == 0)
						handle_exit(cmd, env);
					else if (is_builtin(cmd->args[0]) && run)
						run_builtin(cmd->args, &env, &g_exit);
					else if (cmd->next || cmd->io_fds)
						g_exit = exec_pipeline(cmd, env);
					else
						g_exit = exec_cmd(cmd, env, &g_exit);
				}
			}
			//free_token_list(tok);
			cleanup_all_heredocs(cmd);
			free_cmds(&cmd);
		}
		free(line);
	}
	free_all(NULL, env);
	return (g_exit);
}
