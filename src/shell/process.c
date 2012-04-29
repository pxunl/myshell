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
#include <string.h>
#include <ctype.h>
#include "valuelib.h"
#include "process.h"
#include "config.h"
#include "shell.h"

typedef enum 
{
	NATRULE,
	WANT_THEN,
	THEN_BLOCK
} States;

/*extern struct Value g_Value[MAX_ARGS];*/

static int if_state   = NATRULE;
static int if_result  = R_TRUE;
static int last_state = R_FALSE;


/**
 * @Process 
 *
 * @input: command string
 *
 * @return: True or Fail
 */
int Process(char **input)
{
	int flag = R_FALSE;
	if (NULL == input[0]) 
	{
		return R_FALSE;
	}
	else if (Is_Control_Cmd(input[0]) == R_TRUE) 
	{
		flag = Do_Control_Cmd(input);
	}
	else if (Ok_Execute() == R_TRUE) 
	{
		if (IS_Buildin_Cmd(input) == R_TRUE)
		{
			flag = Process_Buildin_Cmd(input);
		}
		else 
		{
			flag = Execute(input);
		}
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
	if ((strcmp(str, "if") == 0) || (strcmp(str, "then") == 0)  
		|| (strcmp(str, "else") == 0) || (strcmp(str, "fi") == 0 ))
	{
		return R_TRUE;
	}
	return R_FALSE;
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
	int flag = R_FALSE;
	
	//enter if area
	if (strcmp(cmd, "if") == 0)
	{
		if (if_state != NATRULE) 
		{
			Syntax_Error("if unexpected!");
			flag = R_FALSE;
		}
		else 
		{
			/*any backspace in "if_string" is inadmissibility,
			 otherwise it will be divide into a few conditions*/
			last_state = Process(args + 1);
			if_result = last_state;
			if_state = WANT_THEN;
			flag = R_TRUE;
		}
	}
	else if (strcmp(cmd, "then") == 0) 
	{
		if (if_state != WANT_THEN) 
		{
			Syntax_Error("then unexpected");
			flag = R_FALSE;
		}
		else 
		{
			if_state = THEN_BLOCK;
			flag = R_TRUE;
		}
	}
	else if (strcmp(cmd, "fi") == 0) 
	{
		if (if_state != THEN_BLOCK) 
		{
			Syntax_Error("fi unexpected");
			flag = R_FALSE;
		}
		else 
		{
			//enter NATRULE area agian
			if_state = NATRULE;
			flag = R_TRUE;
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
	int flag = R_TRUE;
	
	/*if in THEN_BLOCK then syntax error*/
	if (WANT_THEN == if_state)
	{
		Syntax_Error("then expected");
		flag = R_FALSE;
	}
	/*if in THEN_BLOCK and if_result is R_TRUE then yes else no*/
	else if ((THEN_BLOCK == if_state) && (R_TRUE == if_result))
	{
		flag = R_TRUE;
	}
	else if ((THEN_BLOCK == if_state) && (R_FALSE == if_result))
	{
		flag = R_FALSE;
	}
	
	return flag;
}

void Syntax_Error(char *ckstr)
{
	if_state = NATRULE;
	fprintf(stderr, "syntax error!: %s\n", ckstr);
}

int IS_Buildin_Cmd(char **input)
{
	if (NULL == input) 
	{
		return R_FALSE;
	}
	
	if ((strcmp(input[0], "iset") == 0) 
		|| (strchr(input[0], '=') != NULL) 
		|| (strcmp(input[0], "iexport") == 0))
	{ 
		return R_TRUE;
	} 
	
	return R_FALSE;
}

/**
 * @Process_Buildin_Cmd 
 * description: process the buildin commands of iset , iexport and  string that contains '='
 * @cmd: original command
 */
int Process_Buildin_Cmd(char **cmd)
{
	int flag = R_FALSE;
	if (strcmp(cmd[0], "iset") == 0) 
	{
		Value_List();
		flag = R_TRUE;
	}

	/*we can only export a name not include the it's var*/
	else if (strcmp(cmd[0], "iexport") == 0) 
	{
		if ((cmd[1] != NULL) && (Check_Name(cmd[1]))) 
		{
			flag = Value_Export(cmd[1]);
		}
		else 
		{
			flag = R_FALSE;
		}
	}
	/*if it contains '='*/
	else if (strchr(cmd[0], '=') != NULL) 
	{
		flag = Value_Store(cmd[0]);
	}

	return flag;
}

