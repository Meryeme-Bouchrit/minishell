/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:23:19 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 00:43:33 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**env_to_envp(t_env *env)
{
	int		len;
	char	**envp;
	char	*tmp;
	t_env	*cur;

	len = 0;
	cur = env;
	while (cur && ++len)
		cur = cur->next;
	envp = malloc(sizeof(char *) * (len + 1));
	if (!envp)
		return (NULL);
	len = 0;
	cur = env;
	while (cur)
	{
		tmp = ft_strjoin(cur->key, "=");
		envp[len++] = ft_strjoin(tmp, cur->value);
		free(tmp);
		cur = cur->next;
	}
	envp[len] = NULL;
	return (envp);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:21:37 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 00:28:04 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_path_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*get_path_env(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*find_bin(char *cmd, char **paths)
{
	int		i;
	char	*tmp;
	char	*full;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*full;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path = get_path_envp(envp);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	full = find_bin(cmd, paths);
	free_split(paths);
	return (full);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 02:11:00 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 00:27:59 by mbouchri         ###   ########.fr       */
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:22:10 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 00:27:54 by mbouchri         ###   ########.fr       */
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
        signal(SIGINT, sigint_handler_heredoc);
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
    t_cmd *cur_cmd = cmds;
    t_in_out_fds *cur_redir;
    char *heredoc_filename;

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
                    return 1;
                }
                free(cur_redir->filename);
                cur_redir->filename = heredoc_filename;
            }
            cur_redir = cur_redir->next;
        }

        if (!cur_cmd->args || !cur_cmd->args[0])
        {
            cur_redir = cur_cmd->io_fds;
            while (cur_redir)
            {
                if (cur_redir->type == REDIR_HEREDOC && cur_redir->filename)
                {
                    unlink(cur_redir->filename);
                    free(cur_redir->filename);
                    cur_redir->filename = NULL;
                }
                cur_redir = cur_redir->next;
            }
        }

        cur_cmd = cur_cmd->next;
    }
    return 0;
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:05:04 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 00:27:48 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int g_exit = 0;

void sigint_prompt(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
    g_exit = 130;
}

void sigquit_prompt(int sig)
{
    (void)sig;
}

void sigint_handler_heredoc(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    exit(130);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 09:41:18 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 00:38:41 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_in_out_fds(t_in_out_fds *redir)
{
    t_in_out_fds *tmp;

    while (redir)
    {
        tmp = redir;
        redir = redir->next;
        if (tmp->filename)
        {
            unlink(tmp->filename); // remove heredoc file
            free(tmp->filename);
        }
        free(tmp);
    }
}

void free_all(t_cmd *cmds, t_env *env)
{
    if (cmds)
        free_cmds(&cmds);
    if (env)
        free_env_list(env);
    rl_clear_history();
}

void handle_exit(t_cmd *cmds, t_env *env)
{
    int ret;

    ret = ft_exit(cmds->args, &g_exit);
    if (ret == -1)
    {
        free_all(cmds, env);
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
                    skip_cmd = 1;

                if (!skip_cmd && cmd->args && cmd->args[0])
                {
                    run = (!cmd->next && !cmd->io_fds);

                    if (ft_strcmp(cmd->args[0], "exit") == 0)
                        handle_exit(cmd, env);
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

    free_all(NULL, env);
    return (g_exit);
}
