/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:41:18 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/13 20:17:58 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_status;

static void	handle_exit_builtin(t_cmd *cmds, t_env **env)
{
	int	ret;

	ret = ft_exit(cmds->args, &exit_status);
	if (ret == -1)
	{
		free_env_list(*env);
		exit(exit_status);
	}
}

static int	should_run_builtin_in_main(t_cmd *cmds)
{
	if (!cmds->next && !cmds->io_fds)
		return (1);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	t_token	*tokens;
	t_cmd	*cmds;
	char	*line;
	int		skip_execution;

	(void)argc;
	(void)argv;
	env = copy_env(envp);
	exit_status = 0;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break;
		}

		skip_execution = 0;
		if (only_spaces(line))
			skip_execution = 1;

		if (!skip_execution)
		{
			add_history(line);
			tokens = tokenize(line, env);
			cmds = parse_commands(tokens);

			if (cmds && cmds->args && cmds->args[0])
			{
				if (ft_strcmp(cmds->args[0], "exit") == 0)
					handle_exit_builtin(cmds, &env);
				else if (is_builtin(cmds->args[0]) && should_run_builtin_in_main(cmds))
					run_builtin(cmds->args, &env, &exit_status);
				else if (cmds->next || cmds->io_fds)
					exit_status = exec_pipeline(cmds, env);
				else
					exec_cmd(cmds, env, &exit_status);
			}

			free_token_list(tokens);
			free_cmds(&cmds);
		}

		free(line);
	}

	free_env_list(env);
	return (exit_status);
}
