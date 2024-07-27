#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int is_whitespace(int c)
{
    return ((c >= 9 && c <= 13) || c == 32);
}

t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    cmd->command = NULL;
    return cmd;
}

void initialize_redirection(t_redirection **redirection)
{
    *redirection = malloc(sizeof(t_redirection));
    if (!*redirection)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    (*redirection)->double_left_brace = create_command();
    (*redirection)->double_right_brace = create_command();
    (*redirection)->command = create_command();
    (*redirection)->left_brace = create_command();
    (*redirection)->right_brace = create_command();
}

char **allocate_and_copy(char **cmd, int size)
{
    char **new_cmd = malloc(sizeof(char *) * size);
    if (!new_cmd)
    {
        perror("malloc");
        return NULL;
    }
    for (int j = 0; j < size - 1; j++)
    {
        new_cmd[j] = ft_strdup(cmd[j]);
    }
    return new_cmd;
}

char **append_command(char ***cmd, const char *str)
{
    int i = 0;
    char **new_cmd;

    if (*cmd == NULL)
    {
        new_cmd = malloc(sizeof(char *) * 2);
        if (!new_cmd)
        {
            perror("malloc");
            return NULL;
        }
        new_cmd[0] = ft_strdup(str);
        new_cmd[1] = NULL;
        return new_cmd;
    }
    while ((*cmd)[i])
        i++;
    new_cmd = allocate_and_copy(*cmd, i + 2);
    if (!new_cmd)
        return NULL;
    new_cmd[i] = ft_strdup(str);
    new_cmd[i + 1] = NULL;
    for (int j = 0; j < i; j++)
    {
        free((*cmd)[j]);
    }
    free(*cmd);
    return new_cmd;
}

void free_command_list(char ***command)
{
    if (*command)
    {
        int i = 0;
        while ((*command)[i])
        {
            free((*command)[i]);
            i++;
        }
        free(*command);
        *command = NULL;
    }
}

void parse_left_redirection(char *str, int *i, t_redirection *command)
{
    int j, flag = 0;
    char *content;

    (*i)++;
    if (str[*i] == '<')
    {
        (*i)++;
        flag = 1;
    }
    while (is_whitespace(str[*i]))
        (*i)++;
    j = *i;
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
        (*i)++;
    content = ft_substr(str, j, *i - j);
    if (flag == 1)
        command->double_left_brace->command = append_command(&command->double_left_brace->command, content);
    else
        command->left_brace->command = append_command(&command->left_brace->command, content);
    free(content);
}

void parse_right_redirection(char *str, int *i, t_redirection *command)
{
    int j, flag = 0;
    char *content;

    (*i)++;
    if (str[*i] == '>')
    {
        (*i)++;
        flag = 1;
    }
    while (is_whitespace(str[*i]))
        (*i)++;
    j = *i;
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
        (*i)++;
    content = ft_substr(str, j, *i - j);
    if (flag == 1)
        command->double_right_brace->command = append_command(&command->double_right_brace->command, content);
    else
        command->right_brace->command = append_command(&command->right_brace->command, content);
    free(content);
}

void parse_redirection(char *str, t_redirection *command)
{
    int i = 0;

    while (str[i])
    {
        while (is_whitespace(str[i]))
            i++;
        if (str[i] == '<')
        {
            parse_left_redirection(str, &i, command);
        }
        else if (str[i] == '>')
        {
            parse_right_redirection(str, &i, command);
        }
        i++;
    }
}

void open_redirection_files(t_redirection *command)
{
    int i = 0;
    while (command->left_brace->command && command->left_brace->command[i])
    {
        int fd = open(command->left_brace->command[i], O_WRONLY);
        if (fd == -1)
        {
            perror(command->left_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        close(fd);
        i++;
    }
}

void free_redirection(t_redirection **redirection)
{
    free_command_list(&(*redirection)->double_left_brace->command);
    free_command_list(&(*redirection)->left_brace->command);
    free_command_list(&(*redirection)->double_right_brace->command);
    free_command_list(&(*redirection)->right_brace->command);
    free_command_list(&(*redirection)->command->command);

    free((*redirection)->double_left_brace);
    free((*redirection)->left_brace);
    free((*redirection)->command);
    free((*redirection)->double_right_brace);
    free((*redirection)->right_brace);
    free(*redirection);
    *redirection = NULL;
}

int main(int argc, char **argv, char **envp)
{
    char *str = "  \0";
    t_redirection *command;

    initialize_redirection(&command);
    if (str == NULL)
        exit(EXIT_FAILURE);
    parse_redirection(str, command);
    open_redirection_files(command);
    free_redirection(&command);
    return 0;
}