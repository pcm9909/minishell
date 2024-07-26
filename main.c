#include "main.h"

int ft_isspace(int c)
{
    if ((c >= 9 && c <= 13) || c == 32)
        return 1;
    return 0;
}

t_command *init_command(void)
{
    t_command *cmd;

    cmd = malloc(sizeof(t_command));
    cmd->command = NULL;

    return cmd;
}

void init_redirection(t_redirection **redirection)
{
    (*redirection) = malloc(sizeof(t_redirection));
    (*redirection)->double_left_brace = init_command();
    (*redirection)->double_right_brace = init_command();
    (*redirection)->command = init_command();
    (*redirection)->left_brace = init_command();
    (*redirection)->right_brace = init_command();
}

char **add_command(char ***cmd, char *str)
{
    int i = 0;
    char **new_cmd;

    if (*cmd == NULL)
    {
        new_cmd = malloc(sizeof(char *) * 2);
        if (new_cmd == NULL)
            return NULL;
        new_cmd[0] = ft_strdup(str);
        new_cmd[1] = NULL;
        return new_cmd;
    }
    while ((*cmd)[i])
        i++;
    new_cmd = malloc(sizeof(char *) * (i + 2));
    if (new_cmd == NULL)
        return NULL;
    for (int j = 0; j < i; j++)
        new_cmd[j] = ft_strdup((*cmd)[j]);
    new_cmd[i] = ft_strdup(str);
    new_cmd[i + 1] = NULL;

    for (int j = 0; j < i; j++)
        free((*cmd)[j]);
    free(*cmd);

    return new_cmd;
}

void ft_free1(char ***command)
{
    int i;

    i = 0;
    if ((*command))
    {
        while ((*command)[i])
        {
            free((*command)[i]);
            i++;
        }
    }
    *command = NULL;
    free(*command);
}

int main()
{
    char *str = "<ls < ls < a\0";
    t_redirection *command;
    char *content;
    init_redirection(&command);
    int i = 0;
    int flag = 0;
    int j;

    if (str == NULL)
        exit(0);
    while (str[i])
    {
        flag = 0;
        while (ft_isspace(str[i]))
            i++;
        if (str[i] == '<')
        {
            i++;
            if (str[i] && str[i] == '<')
            {
                i++;
                flag = 1;
            }
            while (ft_isspace(str[i]))
                i++;
            j = i;
            while (str[i] && !ft_isspace(str[i]) && str[i] != '>' && str[i] != '<')
                i++;
            content = ft_substr(str, j, i - j);
            if (flag == 1)
            {
                command->double_left_brace->command = add_command(&command->double_left_brace->command, content);
            }
            else
                command->left_brace->command = add_command(&command->left_brace->command, content);
        }
        else if (str[i] == '>')
        {
            i++;
            if (str[i] && str[i] == '>')
            {
                i++;
                flag = 1;
            }
            while (ft_isspace(str[i]))
                i++;
            j = i;
            while (str[i] && !ft_isspace(str[i]) && str[i] != '>' && str[i] != '<')
                i++;
            content = ft_substr(str, j, i - j);
        }
        else
        {
            j = i;
            while (str[i] && !ft_isspace(str[i]) && str[i] != '>' && str[i] != '<')
                i++;
            content = ft_substr(str, j, i - j);
        }
        free(content);
        i++;
    }
    i = 0;
    while (command->left_brace->command && command->left_brace->command[i])
    {
        int fd = open(command->left_brace->command[i], O_WRONLY);
        {
            if (fd == -1)
            {
                perror(command->left_brace->command[i]);
                exit(1);
            }
        }
        i++;
    }
    ft_free1(&command->double_left_brace->command);
    ft_free1(&command->left_brace->command);
    ft_free1(&command->double_right_brace->command);
    ft_free1(&command->right_brace->command);
    ft_free1(&command->command->command);
    free(command->double_left_brace);
    free(command->left_brace);
    free(command->command);
    free(command->double_right_brace);
    free(command->right_brace);
    free(command);
}