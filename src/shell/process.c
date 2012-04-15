/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  process.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Saturday, April 14, 2012 04:44:07 HKT
 * Description  :  process for if--else--fi  controll
 *
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */

#include <stdio.h>
#include "shell.h"

typedef enum 
{
	NATRULE,
	WANT_THEN,
	THEN_BLOCK
}States;

typedef enum 
{
	SUCCESS,
	FAIL
} Results;


static int if_state   = NATRULE;
static int if_result  = SUCCESS;
static int last_state = 0;

int Do_Control_Command(char **);
int Ok_To_Execute();
int Is_Control_Command(char *str);

	
/**
 * @Process 
 * @process command
 * @input: input command string
 *
 * @return: success or fail
 */
int Process(char **input)
{
	int flag = 0;
	if (NULL == input[0]) 
	{
		flag= 0;
	}
	else if (Is_Contol_Command(input[0])) 
	{
		flag = Do_Control_Command(input);
	}
	else if (Ok_To_Execute()) 
	{
		flag = execute(input);
	}

	return flag;
}
	


/**
 * @Is_Control_Command 
 * @desription: boolean to report if the command is a shell control
 *
 * @return : True or Fail
 */
int Is_Control_Command(char *str)
{
	return ((strcmp(s "if") == 0) || (strcmp(s, "then") == 0) || (strcmp(str, "fi") == 0));
}


/**
 * @Do_Control_Command 
 *
 * @desription: True if ok, fail for anyother error
 *
 * @return True or Fail
 */
int Do_Control_Command(char **input)
{
	char *cmd = input[0];
	int flag = -1;

	if (strcmp(cmd, "if") == 0) 
	{
		if (if_state != NA) 
		{
			
		}
		
	}
	
}
int Ok_To_Execute();
