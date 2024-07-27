#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>

char *local;

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
	cmd->order = false;
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
        if (cmd[j] == NULL)
        {
            new_cmd[j] = NULL;
        }
        else
        {
            new_cmd[j] = ft_strdup(cmd[j]);
            if (!new_cmd[j])
            {
                perror("ft_strdup");
                for (int k = 0; k < j; k++)
                {
                    free(new_cmd[k]);
                }
                free(new_cmd);
                return NULL;
            }
        }
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

void parse_left_redirection(const char *str, int *i, t_redirection *command)
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

char	*ft_strrev(char *str)
{
	int	len;
	int	i;
	char	tmp;

	len = 0;
	i = 0;
	while (str[len])
		len++;
	len -= 1;
	while (i < len)
	{
		tmp = str[i];
		str[i] = str[len];
		str[len] = tmp;
		i++;
		len--;
	}
	return (str);
}

char *find_brace(char *str, char brace)
{
	int i = 0;
	int flag = 0;
	if(str[i] = brace)
	{
		while(is_whitespace(str[i]) && str[i])
		{
			i++;
			flag = 1;
		}
		if(ft_strnstr(str[i], "<<", sizeof(str[i])) || ft_strnstr(str[i], ">>>", sizeof(str[i])) || (flag == 1 && ft_strnstr(str[i], "<", sizeof(str[i]))))
		{
			// bash: 예기치 않은 토큰 `<' 근처에서 문법 오류 만들어야함
			exit(2);
		}


	}
}

void set_order(t_redirection *command, char *str)
{

}

void parse_redirection(char *str, t_redirection *command)
{
    int i = 0;

	set_order(command, str);
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
    }
}
// <<
void open_redirection_files0(t_redirection *command)
{
    int i = 0;
	int cnt;

    while (command->double_left_brace->command && command->double_left_brace->command[i])
    {
		while(1)
		{
			local = readline(">");
			if(ft_strlen(local) == ft_strlen(command->double_left_brace->command[i]) && !ft_strncmp(local, command->double_left_brace->command[i], sizeof(local)))
			{
				free(local);
				break;
			}
			free(local);
		}
        i++;
    }
}


// <
void open_redirection_files1(t_redirection *command)
{
    int i = 0;
    while (command->left_brace->command && command->left_brace->command[i])
    {
        int fd = open(command->left_brace->command[i], O_RDONLY);
        if (fd == -1)
        {
            perror(command->left_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        close(fd);
        i++;
    }
}
// >
void open_redirection_files2(t_redirection *command)
{
	int i = 0;
	while (command->right_brace->command && command->right_brace->command[i])
    {
        int fd = open(command->right_brace->command[i], O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if (fd == -1)
        {
            perror(command->right_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        close(fd);
        i++;
    }
}
// >>
void open_redirection_files3(t_redirection *command)
{
	int i = 0;
	while (command->double_right_brace->command && command->double_right_brace->command[i])
    {
        int fd = open(command->double_right_brace->command[i], O_CREAT | O_APPEND | O_WRONLY, 0644);
        if (fd == -1)
        {
            perror(command->double_right_brace->command[i]);
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
    char *str = "<a <<b <<c <<d <<e\0";
    t_redirection *command;

    initialize_redirection(&command);
    if (str == NULL)
        exit(EXIT_FAILURE);

    parse_redirection(str, command);
	printf("n = %d\n", command->left_brace->order);
	printf("d = %d\n", command->double_left_brace->order);
	// open_redirection_files0(command);
   	// open_redirection_files1(command);
	// open_redirection_files2(command);
	// open_redirection_files3(command);
    free_redirection(&command);
    return 0;
}