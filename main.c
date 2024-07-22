#include "main.h"

t_queue	*init_queue(void)
{
	t_queue	*queue;

	queue = (t_queue *)malloc(sizeof(t_queue));
	//가드
	queue->head = NULL;
	queue->tail = NULL;
	return (queue);
}

t_node	*create_node(char *content)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	//가드
	new_node->content = content;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void	insert_node(t_queue *queue, t_node *node)
{
	if (!queue->head)
	{
		queue->head = node;
		queue->tail = node;
	}
	else
	{
		queue->tail->next = node;
		node->prev = queue->tail;
		queue->tail = node;
	}
}

int ft_isspace(char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
    {
        return (1);
    }
    return (0);
}


int count_fork(char *command)
{
    int cnt_pipe;
    int i = 0;

	if(ft_strlen(command) == 0)
		return -1;
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

void skip_whitespace(char *command, int *i)
{
    while (ft_isspace(command[*i]))
        (*i)++;
}

char *parse_redirection(char *command, int *i)
{
    char *content;
    if (command[*i] == '>')
    {
        if (command[*i + 1] == '>')
        {
            content = ft_strdup(">>");
            (*i) += 2;
        }
        else
        {
            content = ft_strdup(">");
            (*i)++;
        }
    }
    else if (command[*i] == '<')
    {
        if (command[*i + 1] == '<')
        {
            content = ft_strdup("<<");
            (*i) += 2;
        }
        else
        {
            content = ft_strdup("<");
            (*i)++;
        }
    }
    else
    {
        content = NULL;
    }
    return content;
}

char *parse_word(char *command, int *i)
{
    int j = *i;
    while (command[*i] && !ft_isspace(command[*i]) && command[*i] != '>' && command[*i] != '<')
        (*i)++;
    return ft_substr(command, j, *i - j);
}

void tokenize_command(char *command, t_queue **queue)
{
    char *content;
    int i = 0;

    while (command[i])
    {
        skip_whitespace(command, &i);
        if (command[i] == '>' || command[i] == '<')
        {
            content = parse_redirection(command, &i);
        }
        else
        {
            content = parse_word(command, &i);
        }
        if (content)
        {
            insert_node(*queue, create_node(content));
        }
    }
}

int main()
{
    char *str = "ls -al > A > A> A> A> A> A> A>A | ls - al | cat -e";
	// | 로 시작하거나 | 로 끝나는 것들은 처리해야할까?
    char **command = ft_split(str, '|');
	int i = 0;
    t_queue **queue_command = malloc(sizeof(t_queue) * count_fork(str) + 1);
	while(i < count_fork(str) + 1)
	{
		queue_command[i] = init_queue();
		tokenize_command(command[i], &queue_command[i]);
		i++;
	}
	i = 0;
	while(i < count_fork(str) + 1)
	{
		t_node *tmp = queue_command[i]->head;
		while(tmp)
		{
			printf("%s\n", tmp->content);
			tmp = tmp->next;
		}
		i++;
	}

	pid_t pid;

}