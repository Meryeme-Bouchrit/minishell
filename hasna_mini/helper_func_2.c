#include "mini.h"

t_token *new_token_w(const char *val)
{
    t_token *tok = malloc(sizeof(t_token));
    tok->value = strdup(val);
    tok->next = NULL;
    return tok;
}

void add_token_w(t_token **list, const char *val)
{
    t_token *tok = new_token_w(val);
    if (!*list)
        *list = tok;
    else
    {
        t_token *temp = *list;
        while (temp->next)
            temp = temp->next;
        temp->next = tok;
    }
}

int token_list_size(t_token *list)
{
    int count = 0;
    while (list)
    {
        count++;
        list = list->next;
    }
    return count;
}

char **token_list_to_array(t_token *list)
{
    int size = token_list_size(list);
    char **array = malloc(sizeof(char *) * (size + 1));
    int i = 0;
    while (list)
    {
        array[i++] = strdup(list->value);
        list = list->next;
    }
    array[i] = NULL;
    return array;
}

void free_token_list(t_token *list)
{
    while (list)
    {
        t_token *tmp = list;
        list = list->next;
        free(tmp->value);
        free(tmp);
    }
}

t_cmd *new_cmd()
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    cmd->args = NULL;
    cmd->io_fds = NULL;
    cmd->pipe_output = false;
    cmd->next = NULL;
    return cmd;
}

void add_redirection(t_cmd *cmd, t_redirections type, const char *filename)
{
    t_in_out_fds *redir = malloc(sizeof(t_in_out_fds));
    redir->type = type;
    redir->filename = strdup(filename);
    redir->next = NULL;

    if (!cmd->io_fds)
        cmd->io_fds = redir;
    else
    {
        t_in_out_fds *tmp = cmd->io_fds;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = redir;
    }
}

t_cmd *parse_commands(char **tokens)
{
    t_cmd *head = NULL;
    t_cmd *current = new_cmd();
    t_token *temp_args = NULL;

    int i = 0;
    while (tokens[i])
    {
        if (strcmp(tokens[i], "|") == 0)
        {
            current->args = token_list_to_array(temp_args);
            current->pipe_output = true;
            if (!head)
                head = current;
            else
            {
                t_cmd *tmp = head;
                while (tmp->next)
                    tmp = tmp->next;
                tmp->next = current;
            }
            current = new_cmd();
            free_token_list(temp_args);
            temp_args = NULL;
        }
        else if (strcmp(tokens[i], ">") == 0)
        {
            if (tokens[i + 1])
            {
                add_redirection(current, T_REDIR_OUT, tokens[i + 1]);
                i++;
            }
        }
        else
        {
            add_token_w(&temp_args, tokens[i]);
        }
        i++;
    }
    if (temp_args)
    {
        current->args = token_list_to_array(temp_args);
        free_token_list(temp_args);
    }
    if (!head)
        head = current;
    else
    {
        t_cmd *tmp = head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = current;
    }

    return head;
}

void print_commands(t_cmd *cmds)
{
    int num = 1;
    while (cmds)
    {
        printf("=== Command %d ===\n", num++);
        if (cmds->args)
        {
            for (int i = 0; cmds->args[i]; i++)
                printf("Arg[%d]: %s\n", i, cmds->args[i]);
        }
        if (cmds->io_fds)
        {
            t_in_out_fds *redir = cmds->io_fds;
            while (redir)
            {
                printf("Redirection: type %d, file %s\n", redir->type, redir->filename);
                redir = redir->next;
            }
        }
        printf("Pipe output? %s\n\n", cmds->pipe_output ? "Yes" : "No");
        cmds = cmds->next;
    }
}
/*
int main(void)
{
    char *tokens[] = {"echo", "hello world", ">", "out.txt", "|", "cat", "-n", NULL};

    t_cmd *cmds = parse_commands(tokens);
    print_commands(cmds);

    return 0;
}
*/
