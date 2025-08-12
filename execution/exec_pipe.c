/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:22:44 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/12 10:34:04 by mbouchri         ###   ########.fr       */
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
    int i = 0;
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


void    child_process_pipeline(t_cmd *cmd, t_env *env, int **pipes, int i, int total)
{
    int exit_code;

    if (i > 0)
        dup2(pipes[i - 1][0], 0);
    if (i < total - 1)
        dup2(pipes[i][1], 1);
    
    close_pipes(pipes, total - 1);
    
    ft_handle_redirs(cmd->io_fds, env);

    if (is_builtin(cmd->args[0]))
    {
        exit_code = 0;
        run_builtin(cmd->args, &env, &exit_code);
        exit(exit_code);
    }
    else
    {
        char **envp_arr = env_to_envp(env);
        char *path = find_cmd_path(cmd->args[0], envp_arr);
        if (!path)
        {
            perror(cmd->args[0]);
            free_split(envp_arr);
            exit(127);
        }
        execve(path, cmd->args, envp_arr);
        perror("execve");
        free(path);
        free_split(envp_arr);
        exit(127);
    }
}

int exec_pipeline(t_cmd *cmds, t_env *env)
{
    int n;
    int **pipes;
    pid_t *pids;
    t_cmd *cur;
    int i;
    int status;
    char **tmp_heredoc_files;
    int last_exit_code = 0; // ✅ store actual exit code

    tmp_heredoc_files = collect_heredocs(cmds, env);
    if (!tmp_heredoc_files)
        return (1);

    n = 0;
    cur = cmds;
    while (cur && ++n)
        cur = cur->next;

    if (n == 0)
    {
        free_split(tmp_heredoc_files);
        return (0);
    }

    pipes = malloc(sizeof(int *) * (n - 1));
    if (!pipes || create_pipes(pipes, n - 1))
    {
        free_split(tmp_heredoc_files);
        free(pipes);
        return (1);
    }

    pids = malloc(sizeof(pid_t) * n);
    if (!pids)
    {
        close_pipes(pipes, n - 1);
        free_split(tmp_heredoc_files);
        return (1);
    }

    cur = cmds;
    i = -1;
    while (++i < n)
    {
        pids[i] = fork();
        if (pids[i] == 0)
            child_process_pipeline(cur, env, pipes, i, n);
        cur = cur->next;
    }

    close_pipes(pipes, n - 1);

    i = -1;
    while (++i < n)
    {
        waitpid(pids[i], &status, 0);
        if (i == n - 1) // last command’s exit code
            last_exit_code = (status & 0xFF00) >> 8; // ✅ no WIFEXITED
    }

    free_split(tmp_heredoc_files);
    free(pids);
    return (last_exit_code);
}


// exec_pipe.c - Corrected collect_heredocs function
char **collect_heredocs(t_cmd *cmds, t_env *env)
{
    t_cmd *cur = cmds;
    t_in_out_fds *redir;
    int heredoc_count = 0;
    
    while (cur)
    {
        redir = cur->io_fds;
        while (redir)
        {
            if (redir->type == REDIR_HEREDOC)
                heredoc_count++;
            redir = redir->next;
        }
        cur = cur->next;
    }

    char **tmp_files = malloc(sizeof(char *) * (heredoc_count + 1));
    if (!tmp_files)
        return NULL;
    tmp_files[heredoc_count] = NULL;
    
    int i = 0;
    cur = cmds;
    while (cur)
    {
        redir = cur->io_fds;
        while (redir)
        {
            if (redir->type == REDIR_HEREDOC)
            {
                tmp_files[i] = redir_heredoc(redir->filename, env, redir->expand);
                if (!tmp_files[i])
                {
                    while (i-- > 0)
                    {
                        unlink(tmp_files[i]);
                        free(tmp_files[i]);
                    }
                    free(tmp_files);
                    return NULL;
                }
                // Overwrite the filename in the command structure with the new tmp file
                // We assume redir->filename was dynamically allocated and must be freed
                free(redir->filename);
                redir->filename = ft_strdup(tmp_files[i]);
                i++;
            }
            redir = redir->next;
        }
        cur = cur->next;
    }
    return tmp_files;
}
