#include "libft/libft.h"
#include <stdio.h>

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

