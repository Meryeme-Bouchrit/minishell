/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:21:01 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 00:43:40 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void child_process_single(t_cmd *cmd, char *path, t_env *env)
{
    char    **envp_arr;
    
    ft_handle_redirs(cmd->io_fds);
    envp_arr = env_to_envp(env);
    if (!envp_arr)
    {
        perror("env_to_envp");
        exit(1);
    }
    execve(path, cmd->args, envp_arr);
    perror(cmd->args[0]);
    free_split(envp_arr);
    exit(127);
}

int fork_and_wait(t_cmd *cmd, char *path, t_env *env)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    else if (pid == 0)
        child_process_single(cmd, path, env);
    else
    {
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            return (1);
        }
        if ((status & 0x7f) == 0)
            return ((status >> 8) & 0xff);
        else
            return (128 + (status & 0x7f));
    }
    return (1);
}

int exec_cmd(t_cmd *cmd, t_env *env, int *exit_status)
{
    char *path;
    char **envp;
    int fd;

    if (!cmd->args[0] || cmd->args[0][0] == '\0')
    {
       *exit_status = 0;
       return (0);
    }
    if (is_builtin(cmd->args[0]))
        return (run_builtin(cmd->args, &env, exit_status));
    
    if (ft_strchr(cmd->args[0], '/'))
    {
        fd = open(cmd->args[0], O_DIRECTORY);
        if (fd != -1)
        {
            close(fd);
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, ": Is a directory\n", 17);
            *exit_status = 126;
            return (126);
        }
    }
    if (ft_strchr(cmd->args[0], '/') &&
        access(cmd->args[0], F_OK) == 0 &&
        access(cmd->args[0], X_OK) != 0)
    {
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": Permission denied\n", 20);
        *exit_status = 126;
        return (126);
    }
    if (ft_strchr(cmd->args[0], '/') &&
        access(cmd->args[0], F_OK) != 0)
    {
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": No such file or directory\n", 28);
        *exit_status = 127;
        return (127);
    }
    
    envp = env_to_envp(env);
    if (!envp)
    {
        perror("env_to_envp");
        *exit_status = 1;
        return (1);
    }
    path = find_cmd_path(cmd->args[0], envp);
    if (!path)
    {
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": command not found\n", 20);
        free_split(envp);
        *exit_status = 127;
        return (127);
    }
    *exit_status = fork_and_wait(cmd, path, env);
    free_split(envp);
    free(path);
    return (0);
}
