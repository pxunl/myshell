/**
 * shell.c 
 * the main function
 * 2012-3-14
 * By PXunL
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "config.h"
#include "eval.h"

extern int execute(char *argv[]);

void initialize();

int main(int argc, const char *argv[])
{
	char *command_line;
	char **arglist;
	int result;

	initialize();

	while ((command_line = read_command(stdin)) != NULL)
	{	
		if ((arglist = spiltline(command_line)) != NULL) 
		{	
			/*while (*arglist != NULL) */
			/*{*/
				/*printf("%s\n ",*arglist);*/
				/*arglist++;*/
			/*}*/
			result = execute(arglist);
			free_list(arglist);
		}

		free(command_line);
	}
	return 0;
}


/**
 * @purpose:initialize shell
 * @return:nothing
 */
void initialize()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

