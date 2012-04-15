/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  commit.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Saturday, March 03, 2012 12:13:00 HKT
 * Description  :  the main function of my shell 
 *
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "config.h"
#include "eval.h"
#include "execute.c"

#define MAX_ARGS   50
#define ARG_LEN    100

extern int execute(char *argv[]);

void initialize();
iniy
int main(int argc, const char *argv[])
{
	char *command_line;
	char **arglist;
	int  result;

	initialize();

	/*read command and execute them*/
	while ((command_line = read_command(stdin)) != NULL)
	{	
		if ((arglist = spiltline(command_line)) != NULL) 
		{	
			/*while (*arglist != NULL)*/
			/*{*/
				/*printf("%s\n",*arglist);*/
				/*arglist++;*/
			/*}*/
			/*printf("%s\n",*(arglist+1));*/
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

