#include "libft/libft.h"
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>


typedef struct s_command
{
	char	**command;
	bool	order;
	bool	exist;
}				t_command;

typedef struct s_redirection
{
	struct s_command	*double_left_brace;
	struct s_command	*left_brace;
	struct s_command	*command;
	struct s_command	*right_brace;
	struct s_command	*double_right_brace;
}				t_redirection;