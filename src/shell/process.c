/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  process.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Sunday, April 15, 2012 01:39:09 HKT
 * Description  :  It sits in front of the execute() function.
 * 				   For if-then-else-fi scripts control
 *
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include <string.h>


typedef enum 
{
	NATRULE,
	WANT_THEN,
	THEN_BLOCK
} States;

typedef enum 
{
	SUCCESS,
	FAIL
} Results;


static int if_state   = NATRULE;
static int if_result  = SUCCESS;
static int last_state = 0;

int Is_Control_Cmd(char *input);
int Do_Control_Cmd(char **);
int Ok_Execute();
int Syntax_Error(char *ckstr);


/**
 * @Process 
 *
 * @input: command string
 *
 * @return: True or Fail
 */
int Process(char **input)
{
	int flag = 0;

	if (NULL == input[0]) 
	{
		flag = 0;
	}
	else if (Is_Control_Cmd(input[0])) 
	{
		flag = Do_Control_Cmd(input);
	}
	else if (Ok_Execute()) 
	{
		flag = execute(input);
	}
	
	return flag;
}



/**
 * @Is_Control_Cmd 
 *
 * desription: check if *str is if else then fi..eg. control flow
 * @str: input string
 *
 * @return: True or Fail
 */
int Is_Control_Cmd(char *str)
{
	return ((strcmp(str, "if") == 0) || (strcmp(str, "then") == 0)  
			|| (strcmp(str, "else") == 0) || (strcmp(str, "fi") == 0 ));
}




/**
 * @Do_Control_Cmd 
 * description: pross 'if' 'then' 'fi' 'else'  -- change state
 * @args: input string
 *
 * @return: True or Fail
 */
int Do_Control_Cmd(char **args)
{
	char *cmd = args[0];
	int flag = -1;
	
	//enter if area
	if (strcmp(cmd, "if") == 0)
	{
		if (if_state != NATRULE) 
		{
			flag = Syntax_Error("if unexpected!");
		}
		else 
		{
			last_state = Process(args + 1);
			if_result = (last_state == 0)? SUCCESS : FAIL;
			if_state = WANT_THEN;
			flag = 0;
		}
	}
	else if (strcmp(cmd, "then") == 0) 
	{
		if (if_state != WANT_THEN) 
		{
			flag = Syntax_Error("then unexpected");
		}
		else 
		{
			if_state = THEN_BLOCK;
			flag = 0;
		}
	}
	else if (strcmp(cmd, "fi") == 0) 
	{
		if (if_state != THEN_BLOCK) 
		{
			flag = Syntax_Error("fi unexpected");
		}
		else 
		{
			//enter NATRULE area agian
			if_state = NATRULE;
			flag = 0;
		}
	}
	else 
	{
		fatal("internal error:", cmd, 2);
	}
		
	return flag;
}


/**
 * @Ok_Execute 
 * description: 1 for yes, 0 for no
 * 
 * @
 */
int Ok_Execute()
{
	int flag = 1;
	
	/*if in THEN_BLOCK then syntax error*/
	if (WANT_THEN == if_state) 
	{
		Syntax_Error("then expected");
		flag = 0;
	}
	/*if in THEN_BLOCK and if_result is SUCCESS then yes else no*/
	else if ((THEN_BLOCK == if_state) && (SUCCESS == if_result))
	{
		flag = 1;
	}
	else if ((THEN_BLOCK == if_state) && (FAIL == if_result))
	{
		flag = 0;
	}
	
	return flag;
}


int Syntax_Error(char *ckstr)
{
	if_state = NATRULE;
	fprintf(stderr, "syntax error!: %s\n", ckstr);
	return -1;
}

