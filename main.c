/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:41:18 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/12 10:33:20 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exit_status;

void execute_and_free(char *line, t_env **env, int *exit_status)
{
    t_token *tokens;
    t_cmd   *cmds;

    if (!line || only_spaces(line))
    {
        if (line)
            free(line);
        return;
    }
    add_history(line);
    tokens = tokenize(line, *env);
    cmds = parse_commands(tokens);
    if (cmds && cmds->args && cmds->args[0])
    {
        if (cmds->next)
            *exit_status = exec_pipeline(cmds, *env);
        else if (is_builtin(cmds->args[0]))
        {
            if (ft_strcmp(cmds->args[0], "exit") == 0)
            {
                int ret = ft_exit(cmds->args, exit_status);
                free_token_list(tokens);
                free_cmds(&cmds);
                free(line);
                if (ret == -1)
                {
                    free_env_list(*env);
                    exit(*exit_status);
                }
                return;
            }
            else if (cmds->io_fds)
                exec_builtin_with_redir(cmds, env, exit_status);
            else
                run_builtin(cmds->args, env, exit_status);
        }
        else
            exec_cmd(cmds, *env, exit_status); // ✅ fixed: now passing the 3rd argument
    }
    else
        *exit_status = 2; // General error for failed parsing

    free_token_list(tokens);
    free_cmds(&cmds);
    free(line);
}


int main(int argc, char **argv, char **envp)
{
    t_env *env;
    char *line;
    int exit_status;

    env = copy_env(envp);
    exit_status = 0;

    if (argc > 1)
    {
        execute_and_free(ft_strdup(argv[1]), &env, &exit_status);
    }
    else
    {
        while (1)
        {
            line = readline("minishell$ ");
            if (!line)
            {
                write(1, "exit\n", 5);
                break;
            }
            execute_and_free(line, &env, &exit_status);
        }
    }

    free_env_list(env);
    return (exit_status);
}
