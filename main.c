#include "main.h"

char *local;
int pipe_fd[2]; // 전역 변수로 선언

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
        printf("zsh: parse error near `%c'\n", str[*i]);
        exit(258);
    }
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
        (*i)++;
    content = ft_substr(str, j, *i - j);
    if (flag == 1)
    {
        command->double_left_brace->exist = true;
        command->double_left_brace->command = append_command(&command->double_left_brace->command, content);
    }
    else
    {
        command->left_brace->exist = true;
        command->left_brace->command = append_command(&command->left_brace->command, content);
    }
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
        printf("zsh: parse error near `%c'\n", str[*i]);
        exit(258);
    }
    j = *i;
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
        (*i)++;
    content = ft_substr(str, j, *i - j);
    if (flag == 1)
    {
        command->double_right_brace->exist = true;
        command->double_right_brace->command = append_command(&command->double_right_brace->command, content);
    }
    else
    {
        command->right_brace->exist = true;
        command->right_brace->command = append_command(&command->right_brace->command, content);
    }
    free(content);
}

char *handle_double_quotes(const char *str, int *i)
{
    int start = ++(*i);
    while (str[*i] && str[*i] != '"')
        (*i)++;
    if (str[*i] != '"')
    {
        printf("parse error near `\"'\n");
        exit(258);
    }
    char *content = ft_substr(str, start, *i - start);
    (*i)++;
    return content;
}

void parse_command(char *str, int *i, t_redirection *command)
{
    int j;
    char *content;

    while (is_whitespace(str[*i]))
        (*i)++;
    if (str[*i] == '"')
    {
        content = handle_double_quotes(str, i);
    }
    else
    {
        j = *i;
        while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
            (*i)++;
        if (j == *i)
            return;
        content = ft_substr(str, j, *i - j);
    }
    command->command->command = append_command(&command->command->command, content);
    free(content);
}

char *ft_find_single_redirect(char *str, char c)
{
    int i = 0;

    if (!str)
        return NULL;
    while (str[i])
    {
        if (str[i] == c)
        {
            if (str[i + 1] == c)
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
    if (dual > single)
    {
        command->double_left_brace->order = true;
    }
    else if (dual < single)
    {
        command->left_brace->order = true;
    }
    dual = ft_strlen(ft_strnstr(str, ">>", ft_strlen(str)));
    single = ft_strlen(ft_find_single_redirect(str, '>'));
    if (dual > single)
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

void open_redirection_files(t_redirection *command)
{
    int i = 0;
    int fd;

    while (command->double_left_brace->command && command->double_left_brace->command[i])
    {
        char *str = ft_strdup("");
		pid_t pid;

		pid = fork();
		if(pid == 0)
		{
			while (1)
			{
				local = readline(">");
				if (ft_strlen(local) == ft_strlen(command->double_left_brace->command[i]) && !ft_strncmp(local, command->double_left_brace->command[i], sizeof(local)))
				{
					if (command->double_left_brace->order == true)
					{
						dup2(pipe_fd[1], 1);
						write(pipe_fd[1], str, ft_strlen(str));
						close(pipe_fd[0]);
					}
					free(local);
					break;
				}
				if (command->double_left_brace->order)
				{
					str = ft_strjoin(str, local);
				}
				free(local);
			}
		}
		else
    	{
			wait(NULL);
    	}
        i++;
    }

    i = 0;
    while (command->left_brace->command && command->left_brace->command[i])
    {
        fd = open(command->left_brace->command[i], O_RDONLY);
        if (fd == -1)
        {
            perror(command->left_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        if (command->left_brace->order == true)
        {
            dup2(fd, 0);
			//이부분 문제임
			//dup2(pipe_fd[1], 1);
            //close(pipe_fd[0]);
        }
        close(fd);
        i++;
    }

    i = 0;
    while (command->right_brace->command && command->right_brace->command[i])
    {
        fd = open(command->right_brace->command[i], O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if (fd == -1)
        {
            perror(command->right_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        if (command->right_brace->order == true)
        {
            dup2(fd, 1);
        }
        close(fd);
        i++;
    }

    i = 0;
    while (command->double_right_brace->command && command->double_right_brace->command[i])
    {
        fd = open(command->double_right_brace->command[i], O_CREAT | O_APPEND | O_WRONLY, 0644);
        if (fd == -1)
        {
            perror(command->double_right_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        if (command->double_right_brace->order == true)
        {
            dup2(fd, 1);
        }
        close(fd);
        i++;
    }
}

int cnt_cmd(char **split)
{
    int i = 0;

    while (split[i])
    {
        i++;
    }
    return i;
}

void exe(t_redirection *command, char **cmd, char **envp)
{
    char *cmd_path = NULL;
    char *path = get_path(envp);
    if (cmd)
    {
        cmd_path = get_cmd_path(cmd[0], path);
    }
    execve(cmd_path, cmd, NULL);
    free(path);
}

void execute_command(t_redirection *command, char **envp, int input_fd, int output_fd)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // Child process
        if (input_fd != 0)
        {
            dup2(input_fd, 0);
            close(input_fd);
        }
        if (output_fd != 1)
        {
            dup2(output_fd, 1);
            close(output_fd);
        }
        open_redirection_files(command);
        exe(command, command->command->command, envp);
    }
    else
    {
        // Parent process
        // if (input_fd != 0)
        //     close(input_fd);
        // if (output_fd != 1)
        //     close(output_fd);
        wait(NULL);
    }
}

int main(int argc, char **argv, char **envp)
{
    if (argc == 1)
        exit(1);
    char *str = ft_strdup(argv[1]);
    char **split = ft_split(str, '|');
    int cnt = cnt_cmd(split);
    int i = 0;
    t_redirection **command = (malloc(sizeof(t_redirection *) * cnt));
    int input_fd = 0;

    while (split[i])
    {
        initialize_redirection(&command[i]);
        if (str == NULL)
            exit(EXIT_FAILURE);
        parse_redirection(split[i], command[i]);
        print(command[i]);
        i++;
    }

    for (i = 0; i < cnt; i++)
    {
        if (i < cnt - 1)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            execute_command(command[i], envp, input_fd, pipe_fd[1]);
            close(pipe_fd[1]);
            input_fd = pipe_fd[0];
        }
        else
        {
            execute_command(command[i], envp, input_fd, 1);
        }
    }

    i = 0;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
    free(str);

    wait(NULL);

    return 0;
}