/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  builin_command.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Sunday, April 15, 2012 03:36:22 HKT
 * Description  :  process build in command: set export eg.
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
#include "valuelib.h"

extern struct Value g_Value[MAX_VALU_NUM];

int Buildin_Cmd(char **args, int *presult)
{
	int flag = 0;

	if (strcmp(args[0], "iset") == 0) 
	{
		Value_List();
		flag = 1;
	}
	else if (strchr(args[0], '=') != NULL) 
	{
		*presult = assign(args[0]);
		if (*presult != -1) 
		{
			flag = R_OK;
		}
		
	}
	else if (strcmp(args[0], "iexport") == 0) 
	{
		/*if it is vality name, export it*/
		if ((NULL != args[1]) && (Check_Name(args[1]))) 
		{
			flag = Value_Export(args[1]);
		}
		else
		{
			flag = R_OK;
		}
	}

	return flag;
}

/*
 * @assign 
 * description:  execute name=val AND ensure that name is legal
 * @str
 *
 * @returns: -1 for illegal lval, or result of VLstore 
 */
int assign(char *str)
{
	char *cp;
	int	flag = 0;

	cp = strchr(str,'=');
	*cp = '\0';
	rv = (Check_Name(str)? Value_Store(str,cp+1) : -1 );
	*cp = '=';
	return rv;
}


/*
 * purpose: determines if a string is a legal variable name
 * returns: 0 for no, 1 for yes
 */
int Check_Name(char *str)
{
	char	*cp;

	for(cp = str; *cp; cp++ ){
		if ( (isdigit(*cp) && cp==str) || !(isalnum(*cp) || *cp=='_' ))
			return 0;
	}
	return ( cp != str );	/* no empty strings, either */
}
