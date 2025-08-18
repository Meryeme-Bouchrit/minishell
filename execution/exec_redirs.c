/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:22:10 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 06:57:55 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int redir_in_fd(int fd)
{
    if (fd < 0)
        return (1);
    if (dup2(fd, 0) == -1)
    {
        perror("dup2");
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}

int redir_in(char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        write(2, "minishell: ", 12);
        perror(filename);
        return (1);
    }
    return (redir_in_fd(fd));
}


int redir_out(char *filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        write(2, "minishell: ", 12);
        perror(filename);
        return (1);
    }
    if (dup2(fd, 1) == -1)
    {
        perror("dup2");
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}

int redir_app(char *filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        write(2, "minishell: ", 12);
        perror(filename);
        return (1);
    }
    if (dup2(fd, 1) == -1)
    {
        perror("dup2");
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}

void ft_handle_redirs(t_in_out_fds *redir)
{
    int     fd;

    while (redir)
    {
        if (redir->type == REDIR_HEREDOC)
        {
            fd = open(redir->filename, O_RDONLY);
            if (redir_in_fd(fd))
                exit(1);
        }
        else if (redir->type == T_REDIR_IN)
        {
            if (redir_in(redir->filename))
                exit(1);
        }
        else if (redir->type == T_REDIR_OUT)
        {
            if (redir_out(redir->filename))
                exit(1);
        }
        else if (redir->type == REDIR_APPEND)
        {
            if (redir_app(redir->filename))
                exit(1);
        }
        redir = redir->next;
    }
}

int heredoc_child_loop(int out_fd, char *limiter, t_env *env, bool expand)
{
    char *line = NULL;
    char *tmp2 = NULL;
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
        if (expand)
        {
            tmp2 = expand_variables(line, env);
            if (tmp2)
            {
                ft_putstr_fd(tmp2, out_fd);
                ft_putstr_fd("\n", out_fd);
                free(tmp2);
            }
        }
        else
        {
            ft_putstr_fd(line, out_fd);
            ft_putstr_fd("\n", out_fd);
        }
        free(line);
    }
    return 0;
}

char *generate_temp_filename(void)
{
    static int counter = 0;
    char *counter_str;
    char *filename;

    counter_str = ft_itoa(counter++);
    if (!counter_str)
        return (NULL);
    filename = ft_strjoin("/tmp/minishell_heredoc_", counter_str);
    free(counter_str);
    return (filename);
}

char *redir_heredoc(char *limiter, t_env *env, bool expand)
{
    char  *path;
    int   fd;
    pid_t pid;
    int   status;
    void  (*old_int)(int);
    void  (*old_quit)(int);

    path = generate_temp_filename();
    if (!path)
        return (NULL);
    while (1)
    {
        fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd != -1)
            break;
        free(path);
        path = generate_temp_filename();
        if (!path)
            return (NULL);
    }
    old_int = signal(SIGINT, SIG_IGN);
    old_quit = signal(SIGQUIT, SIG_IGN);
    pid = fork();
    if (pid < 0)
    {
        close(fd);
        unlink(path);
        free(path);
        signal(SIGINT, old_int);
        signal(SIGQUIT, old_quit);
        return (NULL);
    }
    if (pid == 0)
    {
        signal(SIGINT, sigint_heredoc);
        signal(SIGQUIT, SIG_IGN);
        heredoc_child_loop(fd, limiter, env, expand);
        close(fd);
        exit(0);
    }
    waitpid(pid, &status, 0);
    signal(SIGINT, old_int);
    signal(SIGQUIT, old_quit);
    close(fd);
    if ((status & 0x7f) != 0)
    {
        if ((status & 0x7f) == 2)
        {
            g_exit = 130;
            unlink(path);
            free(path);
            return (NULL);
        }
    }
    else if (((status >> 8) & 0xff) != 0)
    {
        g_exit = (status >> 8) & 0xff;
        unlink(path);
        free(path);
        return (NULL);
    }
    g_exit = 0;
    return (path);
}

int ft_preprocess_heredocs(t_cmd *cmds, t_env *env)
{
    t_cmd *cur_cmd;
    t_in_out_fds *cur_redir;
    char *heredoc_filename;

    signal(SIGINT, sigint_heredoc);
    signal(SIGQUIT, SIG_IGN);

    cur_cmd = cmds;
    while (cur_cmd)
    {
        cur_redir = cur_cmd->io_fds;
        while (cur_redir)
        {
            if (cur_redir->type == REDIR_HEREDOC)
            {
                heredoc_filename = redir_heredoc(cur_redir->filename, env, cur_redir->expand);
                if (!heredoc_filename)
                {
                    signal(SIGINT, sigint_prompt);
                    signal(SIGQUIT, sigquit_prompt);
                    return (1);
                }
                free(cur_redir->filename);
                cur_redir->filename = heredoc_filename;
            }
            cur_redir = cur_redir->next;
        }
        cur_cmd = cur_cmd->next;
    }

    signal(SIGINT, sigint_prompt);
    signal(SIGQUIT, sigquit_prompt);
    return (0);
}
void cleanup_heredocs(t_cmd *cmds)
{
    t_cmd *c = cmds;
    while (c) {
        t_in_out_fds *r = c->io_fds;
        while (r) 
        {
            if (r->type == REDIR_HEREDOC && r->filename) 
            {
                unlink(r->filename);
            }
            r = r->next;
        }
        c = c->next;
    }
}
