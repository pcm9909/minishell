#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <stdlib.h>

int main()
{
	char *str;
	int i = 0;
	while(1)
	{
		str = readline("hi >> ");
		if(strlen(str) != 0)
			add_history(str);
	}
}