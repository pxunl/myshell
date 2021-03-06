/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  execute.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Saturday, March 03, 2012 11:13:00 HKT
 * Description  :  Execute command by fork a new process
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
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
#include <signal.h>
#include <sys/types.h>
#include "general.h"

#include "config.h"
#include "shell.h"

#include "valuelib.h"

/**
 * @execute 
 * @argv[]: command string
 * @return : R_TRUE or R_FALE
 */
int Execute(char *argv[])
{
	int pid_f;
	int chlid_in = R_FALSE;
	extern char **environ;

	if (argv[0] == NULL)
		return R_FALSE;
	
	/*fork a new procee to execute the command*/
	if ((pid_f = fork()) == -1)
	{
		perror("fork fail");
		/*return R_FALSE;*/
	}

	/*child process*/
	else if (pid_f == 0)
	{
		environ = Table_To_Env();
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("illegal command!\n");
		exit(1);	
	}
	else
	{	
		if (wait(&chlid_in) == -1)
		{
			chlid_in = R_FALSE;
			perror("wait\n");
		}
		chlid_in = (WIFEXITED(chlid_in)? R_TRUE: R_FALSE);
	}

	return chlid_in;
}
