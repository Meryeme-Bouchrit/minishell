/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:47:33 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/12 11:05:32 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// Helper function to check if a string contains only spaces and tabs
int only_spaces(const char *str)
{
    while (*str)
    {
        if (*str != ' ' && *str != '\t')
            return (0);
        str++;
    }
    return (1);
}

// Checks if a command is one of the supported built-ins
int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "echo") == 0)
        return (1);
    if (ft_strcmp(cmd, "cd") == 0)
        return (1);
    if (ft_strcmp(cmd, "pwd") == 0)
        return (1);
    if (ft_strcmp(cmd, "export") == 0)
        return (1);
    if (ft_strcmp(cmd, "unset") == 0)
        return (1);
    if (ft_strcmp(cmd, "env") == 0)
        return (1);
    if (ft_strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}

// Core function to run a built-in command.
// This function acts as a dispatcher, determining which built-in function to call
// and whether it should be treated as a special built-in that affects the parent process.
// It returns -1 for an exit signal, 1 for a failure that shouldn't exit, and 0 for success.
int run_builtin(char **args, t_env **env, int *exit_status)
{
    if (!args || !args[0])
        return (0);

    if (ft_strcmp(args[0], "exit") == 0)
    {
        return (ft_exit(args, exit_status));
    }
    else if (ft_strcmp(args[0], "cd") == 0)
    {
        *exit_status = ft_cd(args, env);
    }
    else if (ft_strcmp(args[0], "export") == 0)
    {
        *exit_status = ft_export(args, env);
    }
    else if (ft_strcmp(args[0], "unset") == 0)
    {
        *exit_status = ft_unset(args, env);
    }
    else if (ft_strcmp(args[0], "echo") == 0)
    {
        *exit_status = ft_echo(args);
    }
    else if (ft_strcmp(args[0], "pwd") == 0)
    {
        *exit_status = ft_pwd();
    }
    else if (ft_strcmp(args[0], "env") == 0)
    {
        *exit_status = ft_env(*env);
    }
    else
    {
        return (0); // Not a built-in
    }
    return (1); // A built-in was executed
}

// Executes a built-in command, handling redirections.
// This function should be called for single commands (not in a pipe).
int exec_builtin_with_redir(t_cmd *cmd, t_env **env, int *exit_status)
{
    int in_backup = dup(0);
    int out_backup = dup(1);
    t_in_out_fds *redir = cmd->io_fds;
    char *heredoc_tmpfile = NULL;
    int result = 0;

    if (in_backup == -1 || out_backup == -1)
    {
        perror("dup");
        return (1);
    }

    while (redir)
    {
        if (redir->type == REDIR_HEREDOC)
        {
            heredoc_tmpfile = redir_heredoc(redir->filename, *env, redir->expand);
            if (!heredoc_tmpfile)
            {
                result = 1;
                break;
            }
            if (redir_in(heredoc_tmpfile) != 0)
            {
                result = 1;
                break;
            }
        }
        else if (redir->type == T_REDIR_IN && redir_in(redir->filename) != 0)
        {
            result = 1;
            break;
        }
        else if (redir->type == T_REDIR_OUT && redir_out(redir->filename) != 0)
        {
            result = 1;
            break;
        }
        else if (redir->type == REDIR_APPEND && redir_app(redir->filename) != 0)
        {
            result = 1;
            break;
        }
        redir = redir->next;
    }

    if (result == 0)
        result = run_builtin(cmd->args, env, exit_status);

    if (dup2(in_backup, 0) == -1)
        perror("dup2");
    if (dup2(out_backup, 1) == -1)
        perror("dup2");

    close(in_backup);
    close(out_backup);

    if (heredoc_tmpfile)
    {
        unlink(heredoc_tmpfile);
        free(heredoc_tmpfile);
    }

    return (result);
}

// Executes a built-in command in a child process, handling redirections.
// This function should be called when the command is part of a pipe.
int exec_builtin_in_child(t_cmd *cmd, t_env **env)
{
    int exit_code;
    
    ft_handle_redirs(cmd->io_fds, *env);
    
    exit_code = 0;
    // We only need to run the built-in, the exit status is set inside
    run_builtin(cmd->args, env, &exit_code);

    return (exit_code);
}
