/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  execute.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Saturday, March 03, 2012 11:13:00 HKT
 * Description  :  
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
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>


#define FAIL  -1
#define TRUE   0

int execute(char *argv[])
{
	int pid_f;
	int chlid_in = -1;

	if (argv[0] == NULL)
		return 0;
	
	/*fork a new procee to execute the command*/
	if ((pid_f = fork()) == -1)
	{
		perror("fork fail\n");
		return FAIL;
	}

	/*child process*/
	else if (pid_f == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("not found!\n");
		exit(1);	
	}

	/*father process*/
	else
	{	
		if (wait(&chlid_in) == -1)
			perror("wait\n");
	}

	/*return chlid_in;*/
	return TRUE;
}
