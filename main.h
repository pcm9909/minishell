#include "libft/libft.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct s_node
{
	char *content;
	struct s_node *next;
	struct s_node *prev;
}				t_node;


typedef struct s_queue
{
	t_node *head;
	t_node *tail;
}				t_queue;

typedef struct s_command
{
	struct t_queue *queue;
	bool exist;
}				t_command;

typedef struct s_redirection
{
	struct s_command *double_left_brace;
	struct s_command *left_brace;
	struct s_command *command;
	struct s_command *right_brace;
	struct s_command *double_right_brace;
}				t_redirection;

