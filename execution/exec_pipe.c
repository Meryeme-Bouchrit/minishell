/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 02:11:00 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 08:18:24 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void    close_pipes(int **pipes, int n)
{
    int i;

    i = 0;
    while (i < n)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        free(pipes[i]);
        i++;
    }
    free(pipes);
}

int create_pipes(int **pipes, int n)
{
    int i;

    i = 0;
    while (i < n)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i] || pipe(pipes[i]) == -1)
        {
            perror("pipe");
            while (--i >= 0)
            {
                close(pipes[i][0]);
                close(pipes[i][1]);
                free(pipes[i]);
            }
            return (1);
        }
        i++;
    }
    return (0);
}

void child_process_pipeline(t_cmd *cmd, t_env *env, int **pipes, int i, int total)
{
    int     exit_code;
    char    **envp_arr;
    char    *path;
    int     fd;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    exit_code = 0;
    if (i > 0)
        dup2(pipes[i - 1][0], 0);
    if (i < total - 1)
        dup2(pipes[i][1], 1);
    close_pipes(pipes, total - 1);

    ft_handle_redirs(cmd->io_fds);

    if (is_builtin(cmd->args[0]))
    {
        run_builtin(cmd->args, &env, &exit_code);
        exit(exit_code);
    }

    if (!cmd->args[0] || cmd->args[0][0] == '\0')
        exit(0);
    if (ft_strchr(cmd->args[0], '/'))
    {
        fd = open(cmd->args[0], O_DIRECTORY);
        if (fd != -1)
        {
            close(fd);
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, ": Is a directory\n", 17);
            exit(126);
        }
    }
    if (ft_strchr(cmd->args[0], '/') &&
        access(cmd->args[0], F_OK) == 0 &&
        access(cmd->args[0], X_OK) != 0)
    {
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": Permission denied\n", 20);
        exit(126);
    }
    if (ft_strchr(cmd->args[0], '/') &&
        access(cmd->args[0], F_OK) != 0)
    {
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": No such file or directory\n", 28);
        exit(127);
    }
    envp_arr = env_to_envp(env);
    path = find_cmd_path(cmd->args[0], envp_arr);
    if (!path)
    {
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": command not found\n", 20);
        free_split(envp_arr);
        exit(127);
    }
    execve(path, cmd->args, envp_arr);
    perror(cmd->args[0]);
    free(path);
    free_split(envp_arr);
    exit(127);
}

int exec_pipeline(t_cmd *cmds, t_env *env)
{
    int n;
    int **pipes;
    pid_t *pids;
    t_cmd *cur;
    int i;
    int status;
    int last_exit_code;
    int ret_code;

    n = 0;
    pipes = NULL;
    pids = NULL;
    ret_code = 0;
    last_exit_code = 0;

    signal(SIGINT, sigint_prompt);
    signal(SIGQUIT, SIG_IGN);

    cur = cmds;
    while (cur && ++n)
        cur = cur->next;

    if (n == 0)
        return (0);
    if (n > 1)
    {
        pipes = malloc(sizeof(int *) * (n - 1));
        if (!pipes || create_pipes(pipes, n - 1))
        {
            if (pipes)
                free(pipes);
            return (1);
        }
    }
    pids = malloc(sizeof(pid_t) * n);
    if (!pids)
    {
        if (pipes)
            close_pipes(pipes, n - 1);
        return (1);
    }
    cur = cmds;
    i = -1;
    while (++i < n)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            ret_code = 1;
            break;
        }
        if (pids[i] == 0)
            child_process_pipeline(cur, env, pipes, i, n);
        cur = cur->next;
    }
    if (pipes)
        close_pipes(pipes, n - 1);

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    i = -1;
    while (++i < n)
    {
        if (pids[i] > 0)
        {
            if (waitpid(pids[i], &status, 0) > 0 && i == n - 1)
            {
                if ((status & 0x7f) == 0)
                    last_exit_code = (status >> 8) & 0xff;
                else
                {
                    int sig = (status & 0x7f);
                    if (sig == SIGINT)
                    {
                        write(1, "\n", 1);
                        last_exit_code = 130;
                    }
                    else if (sig == SIGQUIT)
                    {
                        write(2, "Quit (core dumped)\n", 19);
                        last_exit_code = 131;
                    }
                    else
                        last_exit_code = 128 + sig;
                }
            }
        }
    }

    signal(SIGINT, sigint_prompt);
    signal(SIGQUIT, SIG_IGN);

    free(pids);
    if (ret_code == 0)
        return (last_exit_code);
    return (ret_code);
}
