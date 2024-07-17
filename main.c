#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>

int	ft_isspace(char c)
{
	if(c >= 9 && c <= 13 || c == 32)
	{
		return (1);
	}
	return (0);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	min_size;

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
	while(command[i])
	{
		if(*command == '|')
		{
			cnt_pipe++;
		}
		i++;
	}
	return cnt_pipe;
}

// t_list	*tokenize_command(char *command)
// {
// 	char	**token;
// 	t_list	*cmd;
// 	int		i;

// 	i = 0;
// 	while(command[i])
// 	{
// 		while(ft_isspace(command[i]))
// 		{
// 			i++;
// 		}
// 		if(command[i] == '>')
// 		{
// 			if(command[i + 1] == '>' && command[i + 2] != '>' || command[i + 2] != '<')
// 			{
// 				cmd = ft_lstnew('>>');
// 			}
// 			else
// 			{
// 				cmd = ft_lstnew('>');
// 			}
// 		}
// 		if(command[i] == '<')
// 		{
// 			if(command[i + 1] == '<' && command[i + 2] != '<' || command[i + 2] != '>')
// 			{
// 					cmd = ft_lstnew('<<');
// 			}
// 			else
// 			{
// 				cmd = ft_lstnew('<');
// 			}
// 		}
// 		break;
// 	}
// 	return cmd;
// }

t_list *tokenize_command(char *command)
{
    t_list *cmd ;
    char *content;
    int i = 0;

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
            if (command[i + 1] == '<') {
                content = ft_strdup("<<");
                i += 2;
            }else
			{
                content = ft_strdup("<");
                i++;
            }
        }
		else
		{
            continue;
        }
        cmd = ft_lstnew(content);
        if (!cmd)
		{
            return NULL;
        }
        break;
    }
    return cmd;
}


// | 을 기준으로 split 해주기

int main()
{
	char *str = "<<";
	char **command = ft_split(str, '|');
	t_list *lst = tokenize_command(command[0]);
	//count_fork(str);
	printf("%s\n",lst->content);
}