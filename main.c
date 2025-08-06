/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:41:18 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/06 04:06:55 by zhassna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_env   *env;
    t_token *tokens;
    t_cmd   *cmds;
    char    *line;
    int     exit_status;

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
        if (only_spaces(line))
        {
            free(line);
        }
        else
        {
            add_history(line);
            tokens = tokenize(line, env);
	    cmds = parse_commands(tokens);
            if (cmds && cmds->args && cmds->args[0])
            {
                if (is_builtin(cmds->args[0]))
                {
                    if (ft_strcmp(cmds->args[0], "exit") == 0)
                    {
                        int ret = ft_exit(cmds->args, &exit_status);
                        free_token_list(tokens);
                        free_cmds(&cmds);
                        free(line);
                        if (ret == -1)
                        {
                            free_env_list(env);
                            exit(exit_status);
                        }
                    }
                    else if (cmds->io_fds)
                    {
                        exec_builtin_with_redir(cmds, &env, &exit_status);
                        free_token_list(tokens);
                        free_cmds(&cmds);
                        free(line);
                    }
                    else
                    {
                        run_builtin(cmds->args, &env, &exit_status);
                        free_token_list(tokens);
                        free_cmds(&cmds);
                        free(line);
                    }
                }
                else
                {
                    exit_status = ft_execute_cmd(cmds, envp);
                    free_token_list(tokens);
                    free_cmds(&cmds);
                    free(line);
                }
            }
            else
            {
                free_token_list(tokens);
                free_cmds(&cmds);
                free(line);
            }
        }
    }
    free_env_list(env);
    return (exit_status);
}

