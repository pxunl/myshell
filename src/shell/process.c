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
#include <config.h>


typedef enum 
{
	NATRULE,
	WANT_THEN,
	THEN_BLOCK
} States;


static int if_state   = NATRULE;
static int if_result  = R_OK;
static int last_state = 0;

int Is_Control_Cmd(char *input);
int Do_Control_Cmd(char **);
int Ok_Execute();
void Syntax_Error(char *ckstr);
int Is_Buildin_Cmd(char **input);
void Process_Buildin_Cmd(char **cmd);


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
		flag = R_FALSE;
	}
	else if (Is_Control_Cmd(input[0])) 
	{
		flag = Do_Control_Cmd(input);
	}
	else if (Ok_Execute()) 
	{
		if (Is_Buildin_Cmd()) 
		{
			Process_Buildin_Cmd(input);
		}
		else 
		{
			flag = execute(input);
		}
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
	if ((strcmp(str, "if") == 0) || (strcmp(str, "then") == 0)  
		|| (strcmp(str, "else") == 0) || (strcmp(str, "fi") == 0 ))
	{
		return R_OK;
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
			last_state = Process(args + 1);
			if_result = last_state;
			if_state = WANT_THEN;
			flag = R_OK;
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
			flag = R_OK;
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
			flag = R_OK;
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
	int flag = R_FALSE;
	
	/*if in THEN_BLOCK then syntax error*/
	if (WANT_THEN == if_state) 
	{
		Syntax_Error("then expected");
		flag = R_FALSE;
	}
	/*if in THEN_BLOCK and if_result is R_OK then yes else no*/
	else if ((THEN_BLOCK == if_state) && (R_OK == if_result))
	{
		flag = R_OK;
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
	if ((input[0], "set") == 0) 
		|| (strchr(input[0], '=') != NULL) 
		|| (strcmp(input[0], "export")) 
	{ 
		return R_OK;
	} 
	else 
	{
		return R_FALSE;
	}
	
	return R_FALSE;
}
	

void Process_Buildin_Cmd(char **cmd)
{
	if (strcmp(cmd[0], "iset") == 0) 
	{
		Value_List();
	}
	else if (strcmp(cmd[0], "iexport") == 0) 
	{
		if ((cmd[1] != NULL) && (Check_Name(args[1]))) 
		{
			Value_Export(cmd[1]);
		}
		
	}
	else if (strchr(cmd[0], '=') != NULL) 
	{
		Value_Assign(cmd);
	}
}

