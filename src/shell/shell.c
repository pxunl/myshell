/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  commit.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Saturday, March 03, 2012 12:13:00 HKT
 * Description  :  the main function of my shell, Shell_Main() call function Process to
 * 				   process command lines(string), spilt command line through function spiltline
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
#include "process.h"

void Shell_Main(char *command_line)
{
	if (!command_line) 
	{
		printf("error, no commanline!");
		return;
	}
	char **arglist;
	int  result = 0;

	if ((arglist = spiltline(command_line)) != NULL)
	{	
		result = Process(arglist);
		free_list(arglist);
	}
}
