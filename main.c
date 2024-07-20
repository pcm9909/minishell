#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>

int ft_isspace(char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
    {
        return (1);
    }
    return (0);
}

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void *new_ptr;
    size_t min_size;

    if (ptr == NULL)
        return (malloc(new_size));
    if (new_size == 0)
        return (NULL);
    new_ptr = malloc(new_size);
    if (new_ptr)
    {
        if (old_size < new_size)
            min_size = old_size;
        else
            min_size = new_size;
        ft_memcpy(new_ptr, ptr, min_size);
        free(ptr);
    }
    return (new_ptr);
}

int count_fork(char *command)
{
    int cnt_pipe;
    int i = 0;

    cnt_pipe = 0;
    while (command[i])
    {
        if (command[i] == '|')
        {
            cnt_pipe++;
        }
        i++;
    }
    return cnt_pipe;
}

t_list *tokenize_command(char *command, t_list **lst)
{
    t_list *cmd;
    char *content;
    int i = 0;
    int j = 0;

    while (command[i])
    {
        while (ft_isspace(command[i]))
        {
            i++;
        }
        if (command[i] == '>')
        {
            if (command[i + 1] == '>')
            {
                content = ft_strdup(">>");
                i += 2;
            }
            else
            {
                content = ft_strdup(">");
                i++;
            }
        }
        else if (command[i] == '<')
        {
            if (command[i + 1] == '<')
            {
                content = ft_strdup("<<");
                i += 2;
            }
            else
            {
                content = ft_strdup("<");
                i++;
            }
        }
        else
        {
            j = i;
            while (command[i] && !ft_isspace(command[i]) && command[i] != '>' && command[i] != '<')
            {
                i++;
            }
            content = ft_substr(command, j, i - j);
        }
        ft_lstadd_back(lst, ft_lstnew(content));
        if (!cmd)
        {
            return NULL;
        }
    }
    return cmd;
}

int main()
{
    char *str = "<<a ls -al > a | ls -al | cat | cat | cat";
    char **command = ft_split(str, '|');
    t_list *lst;
	int i = 0;
	while(command[i])
	{
		printf("%d\n", i);
    	lst = tokenize_command(command[i], &lst);
		i++;
	}
	// while(lst)
	// {
	// 	printf("%s ", lst->content);
	// 	lst = lst->next;
	// }
    for (int i = 0; command[i]; i++)
    {
        free(command[i]);
    }
    free(command);
}