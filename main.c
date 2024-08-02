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
	cmd->exist = false;
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

//realloc
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
    if (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
    {
        //에러 출력부 변경필요!
        printf("zsh: parse error near `%c'\n", str[*i]);
        exit(258);
    }
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
    if (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
    {
        //에러 출력부 변경필요!
        printf("zsh: parse error near `%c'\n", str[*i]);
        exit(258);
    }
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

void parse_command(char *str, int *i, t_redirection *command)
{
    int j;
    char *content;

    while (is_whitespace(str[*i]))
        (*i)++;
    j = *i;
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
        (*i)++;
	if(j == *i)
		return;
    content = ft_substr(str, j, *i - j);
    command->command->command = append_command(&command->command->command, content);
    free(content);
}

char *ft_strrev(char *str)
{
	int i = 0;
	int len = ft_strlen(str);
	char *rev = malloc(len + 1);

	rev[len] = '\0';
	while(str[i])
	{
		rev[i] = str[len - i - 1];
		i++;
	}
	return rev;
}

char *ft_find_single_redirect(char *str, char c)
{
	int i = 0;

	if(!str)
		return NULL;
	while(str[i])
	{
		if(str[i] == c)
		{
			if(str[i + 1] == c)
			{
				i++;
			}
			else
			{
				return &str[i];
			}
		}
		i++;
	}
	return NULL;
}

void set_order(t_redirection *command, char *str)
{
	int dual;
	int single;

	dual = ft_strlen(ft_strnstr(str, "<<", sizeof(str)));
	single = ft_strlen(ft_find_single_redirect(str, '<'));
	if(dual > single)
	{
		command->double_left_brace->order = true;
	}
	else if (dual < single)
	{
		command->left_brace->order = true;
	}
	dual = ft_strlen(ft_strnstr(str, ">>", ft_strlen(str)));
	single = ft_strlen(ft_find_single_redirect(str, '>'));
	if(dual > single)
	{
		command->double_right_brace->order = true;
	}
	else if (dual < single)
	{
		command->right_brace->order = true;
	}
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
        else
        {
            parse_command(str, &i, command);
        }
    }
	set_order(command, str);
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
			write(1, local, ft_strlen(local));
			if(ft_strlen(local) == ft_strlen(command->double_left_brace->command[i]) && !ft_strncmp(local, command->double_left_brace->command[i], sizeof(local)))
			{
				printf("in\n");
				free(local);
				break;
			}
			free(local);
		}
        i++;
    }
}


// <
void open_redirection_files1(t_redirection *command, int *fd)
{
    int i = 0;
    while (command->left_brace->command && command->left_brace->command[i])
    {
        *fd = open(command->left_brace->command[i], O_RDONLY);
        if (*fd == -1)
        {
            perror(command->left_brace->command[i]);
            exit(EXIT_FAILURE);
        }
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

int cnt_cmd(char **split)
{
	int i = 0;

	while(split[i])
	{
		i++;
	}
	return i;
}

char	*get_path(char **envp)
{
	int		i;
	char	*path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			return (path);
		}
		i++;
	}
	return (NULL);
}

void	check_cmd(char *cmd_path)
{
	if (!cmd_path)
	{
		perror("command not found");
		exit(1);
	}
}

char	*check_path(char *cmd)
{
	if (cmd[0] == '/')
	{
		if (access(cmd, X_OK) != -1)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	return (NULL);
}

char	*search_path(char *cmd, char *path)
{
	char	**paths;
	char	*cmd_path;
	char	*tmp;
	int		i;

	paths = ft_split(path, ':');
	cmd_path = NULL;
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(tmp, cmd);
		if (access(cmd_path, X_OK) != -1)
			break ;
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	i = 0;
	free(paths);
	return (cmd_path);
}

char	*get_cmd_path(char *cmd, char *path)
{
	char	*cmd_path;

	cmd_path = check_path(cmd);
	if (cmd_path != NULL)
		return (cmd_path);
	return (search_path(cmd, path));
}

void exe(t_redirection *command, char **cmd, char **envp)
{
	int fd;
	char *cmd_path = NULL;
	open_redirection_files0(command);
	open_redirection_files1(command, &fd);
	char *path = get_path(envp);
	if(cmd)
	{
		cmd_path = get_cmd_path(cmd[0], path);
	}
	dup2(fd, 0);
	execve(cmd_path, cmd, NULL);
	free(path);
}

void print(t_redirection *cmd)
{
	int i = 0;
	if(cmd->left_brace->command)
	{
		while(cmd->left_brace->command[i])
		{
			printf("[lb]\n");
			printf("%s\n", cmd->left_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->double_left_brace->command)
	{
		while(cmd->double_left_brace->command[i])
		{
			printf("[dlb]\n");
			printf("%s\n", cmd->double_left_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->command->command)
	{
		while(cmd->command->command[i])
		{
			printf("[cmd]\n");
			printf("%s\n", cmd->command->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->right_brace->command)
	{
		while(cmd->right_brace->command[i])
		{
			printf("[rb]\n");
			printf("%s\n", cmd->right_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->double_right_brace->command)
	{
		while(cmd->double_right_brace->command[i])
		{
			printf("[drb]\n");
			printf("%s\n", cmd->double_right_brace->command[i]);
			i++;
		}
	}
}

int main(int argc, char **argv, char **envp)
{
	if(argc == 1)
		exit(1);
    char *str = ft_strdup(argv[1]);
	char **split = ft_split(str, '|');
	int cnt = cnt_cmd(split);
	int i = 0;
    t_redirection **command = (malloc(sizeof(t_redirection *) * cnt));

	while(split[i])
	{
		initialize_redirection(&command[i]);
		if (str == NULL)
			exit(EXIT_FAILURE);
		parse_redirection(split[i], command[i]);

		print(command[i]);

		exe(command[i] ,command[i]->command->command,envp);

		// open_redirection_files2(command[i]);
		// open_redirection_files3(command[i]);

		// free_redirection(&command[i]);
		i++;
	}
	i = 0;
	while(split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	free(str);
    return 0;
}

//만약에 실패시 dup2사용 아니면
.