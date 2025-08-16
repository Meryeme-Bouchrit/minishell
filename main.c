/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:41:18 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/16 02:11:38 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_exit_builtin(t_cmd *cmds, t_env **env)
{
    int ret;

    ret = ft_exit(cmds->args, &g_exit);
    if (ret == -1)
    {
        free_env_list(*env);
        exit(g_exit);
    }
}

int main(int argc, char **argv, char **envp)
{
    t_env   *env;
    t_token *tokens;
    t_cmd   *cmds;
    char    *line;
    int     skip_execution;
    int     run;

    (void)argc;
    (void)argv;
    env = copy_env(envp);
    g_exit = 0;

    signal(SIGINT, sigint_prompt);
    signal(SIGQUIT, SIG_IGN);
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
                run = (!cmds->next && !cmds->io_fds);

                if (ft_strcmp(cmds->args[0], "exit") == 0)
                    handle_exit_builtin(cmds, &env);
                else if (is_builtin(cmds->args[0]) && run)
                    run_builtin(cmds->args, &env, &g_exit);
                else if (cmds->next || cmds->io_fds)
                    g_exit = exec_pipeline(cmds, env);
                else
                    exec_cmd(cmds, env, &g_exit);
            }

            free_token_list(tokens);
            free_cmds(&cmds);
        }
        free(line);
    }
    free_env_list(env);
    return (g_exit);
}
