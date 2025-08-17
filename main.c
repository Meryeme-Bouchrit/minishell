/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:41:18 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/17 23:56:18 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_exit(t_cmd *cmds, t_env **env)
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
    t_token *tok;
    t_cmd   *cmd;
    char    *line;
    int     skip;
    int     run;
    int     skip_cmd;

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
        skip = only_spaces(line);
        skip_cmd = 0;
        if (!skip)
        {
            add_history(line);
            tok = tokenize(line, env);
            cmd = parse_commands(tok);
            if (cmd)
            {
                if (ft_preprocess_heredocs(cmd, env) != 0)
                {
                    skip_cmd = 1;
                }
                if (!skip_cmd && cmd->args && cmd->args[0])
                {
                    run = (!cmd->next && !cmd->io_fds);

                    if (ft_strcmp(cmd->args[0], "exit") == 0)
                        handle_exit(cmd, &env);
                    else if (is_builtin(cmd->args[0]) && run)
                        run_builtin(cmd->args, &env, &g_exit);
                    else if (cmd->next || cmd->io_fds)
                        g_exit = exec_pipeline(cmd, env);
                    else
                        exec_cmd(cmd, env, &g_exit);
                }
            }
            free_token_list(tok);
            cleanup_heredocs(cmd);
            free_cmds(&cmd);
        }
        free(line);
    }
    free_env_list(env);
    return (g_exit);
}
