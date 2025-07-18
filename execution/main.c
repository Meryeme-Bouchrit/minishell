/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:47:33 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/15 16:43:35 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// #include <readline/readline.h>

int	run_builtin(char **args, t_env **env)
{
	int	exit_status = 0;

	if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "cd") == 0)
		ft_cd(args, env);
	else if (ft_strcmp(args[0], "echo") == 0)
		ft_echo(args);
	else if (ft_strcmp(args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(args[0], "env") == 0)
		ft_env(*env);
	else if (ft_strcmp(args[0], "unset") == 0)
		ft_unset(args, env);
	else if (ft_strcmp(args[0], "export") == 0)
		ft_export(args, env);
	else if (ft_strcmp(args[0], "exit") == 0)
	{
		if (ft_exit(args, &exit_status) != 1)
		{
			free_split(args);
			free_env_list(*env);
			exit(exit_status);
		}
	}
	else
		return (0);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	char	**args;
	t_env	*env;
	t_cmd	tmp_cmd;

	(void)ac;
	(void)av;
	env = copy_env(envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break;
		args = ft_split(line, ' ');
		if (args && args[0])
		{
			if (!run_builtin(args, &env))
			{
				tmp_cmd.args = args;
				tmp_cmd.io_fds = NULL;
				tmp_cmd.pipe_output = 0;
				tmp_cmd.next = NULL;
				ft_execute_cmd(&tmp_cmd, envp);
			}
		}
		if (args)
			free_split(args);
		free(line);
	}
	free_env_list(env);
	return (0);
}
