/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:22:10 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/14 10:19:56 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char *generate_temp_filename(void)
{
    static int  counter = 0;
    char        *num_str;
    char        *filename;

    num_str = ft_itoa(counter++);
    if (!num_str)
        return (NULL);
    filename = ft_strjoin("/tmp/minishell_heredoc_", num_str);
    free(num_str);
    return (filename);
}

char *redir_heredoc(char *limiter, t_env *env, bool expand)
{
    char    *tmp_filename;
    int     fd;
    char    *line;
    char    *processed_line;

    tmp_filename = generate_temp_filename();
    if (!tmp_filename)
        return (NULL);

    fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1)
    {
        perror("open");
        free(tmp_filename);
        return (NULL);
    }

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
            processed_line = expand_variables(line, env);
            if (processed_line)
            {
                ft_putstr_fd(processed_line, fd);
                free(processed_line);
            }
        }
        else
            ft_putstr_fd(line, fd);
        ft_putstr_fd("\n", fd);
        free(line);
    }
    close(fd);
    return (tmp_filename);
}

static int redir_in_fd(int fd)
{
    if (fd < 0)
        return (1);
    if (dup2(fd, STDIN_FILENO) == -1)
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
        perror(filename);
        return (1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
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
        perror(filename);
        return (1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}

int ft_handle_redirs(t_in_out_fds *redir)
{
    while (redir)
    {
        if (redir->type == REDIR_HEREDOC)
        {
            if (redir_in(redir->filename))
                return (1);
        }
        else if (redir->type == T_REDIR_IN)
        {
            if (redir_in(redir->filename))
                return (1);
        }
        else if (redir->type == T_REDIR_OUT)
        {
            if (redir_out(redir->filename))
                return (1);
        }
        else if (redir->type == REDIR_APPEND)
        {
            if (redir_app(redir->filename))
                return (1);
        }
        redir = redir->next;
    }
    return (0);
}
