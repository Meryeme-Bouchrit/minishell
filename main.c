/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:41:18 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/24 18:25:13 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	set_sig(void)
{
	signal(SIGINT, sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
}

void	process_line(char *line, t_env **env)
{
	t_token	*tok;
	t_cmd	*cmd;

	tok = tokenize(line, *env);
	cmd = parse_commands(tok);
	free_token_list(tok);
	check_if_cmd(cmd, env);
	heredocs_cleanup(cmd);
	free_cmds(&cmd);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*line;

	(void)argc;
	(void)argv;
	env = dup_env(envp);
    if (!env)
    {
        env = NULL;
        env_add(&env, ft_strdup("PATH"), ft_strdup("/usr/bin:/bin"));
    }
	g_exit = 0;
	set_sig();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (!only_spaces(line))
		{
			add_history(line);
			process_line(line, &env);
		}
		free(line);
	}
	free_all(NULL, env);
	return (g_exit);
}
