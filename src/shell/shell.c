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
#include <sys/wait.h>


#include "config.h"
#include "eval.h"
#include "shell.h"
#include "valuelib.h"
/*#include "execute.c"*/

extern int execute(char *argv[]);

void initialize();

void Shell_Main(char *command_line)
{
	if (!command_line) 
	{
		return;
	}
	
	/*char *command_line;*/
	char **arglist;
	int  result = 0;

	initialize();

	/*read command and execute them*/
	/*while ((command_line = read_command(stdin)) != NULL)
	{	
		if ((arglist = spiltline(command_line)) != NULL)
		{	
			while (*arglist != NULL)
			{
				printf("%s\n",*arglist);
				arglist++;
			}
			printf("%s\n",*(arglist+1));
			result = execute(arglist);
			result = Process(arglist);
			free_list(arglist);
		}
		free(command_line);
	}*/
	
	if ((arglist = spiltline(command_line)) != NULL)
	{	
		result = Process(arglist);
		free_list(arglist);
	}
	/*free(command_line);*/
}


/**
 * @purpose:initialize shell
 * @return:nothing
 */
void initialize()
{
	extern char **environ;
	Env_To_Table(environ);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

