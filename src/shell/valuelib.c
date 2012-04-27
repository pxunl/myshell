/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  valuelib.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Monday, April 16, 2012 03:53:50 HKT
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
#include <ctype.h>
#include <string.h>
#include "config.h"
#include "shell.h"
#include "valuelib.h"

struct Value
{
	char *str;
	int  global;
};

struct valuelib_t 
{
	/* data */
} /* optional variable list */;

static struct Value g_Value[MAX_ARGS];


/**
 * @Value_List : list all the enviroment values
 * 				 global values will be marked with '*'
 */
void Value_List()
{
	int i;
	for (i = 0; (i < MAX_ARGS && g_Value[i].str != NULL); i++) 
	{
		if (g_Value[i].global) 
		{
			printf(" * %s\n", g_Value[i].str);
		}
		else
		{
			printf("   %s\n", g_Value[i].str);
		}
	}
	
}

void Value_Export(char *name)
{
	return (void)0;
}


/**
 * @Find_Item : find item by name in g_Value
 * @name: var.name
 * @return: address that should be change it's value
 */
static struct Value *Find_Item( char *name)
{
	int i = 0;
	int len = strlen(name);
	
	while ((i < MAX_ARGS) && (g_Value[i].str != NULL))
	{
		if (strncmp(g_Value[i].str, name, len) == 0) 
		{
			return &g_Value[i];
		}
		i++;
	}
	
	return &g_Value[i];
}

/*
 * returns new string of form name=value or NULL on error
 */
char *Create_Var( char *name, char *var )
{
	char	*add_var;

	add_var = malloc( strlen(name) + strlen(var) + 2);
	if (add_var != NULL)
		sprintf(add_var, "%s=%s", name, var);
	return add_var;
}


int Value_Store(char *var)
{
	if (!var) 
	{
		return R_FALSE;
	}
	
	int flag = R_FALSE;

	char *name = var;
	char *value = var;
	char *equal = NULL;
	struct Value *item = NULL;
	char *new_var;

	equal = strchr(var, '=');
	*equal = '\0';
	value = equal + 1;

	if ((!name) || (!value) || (!Check_Name(name))) 
	{
		return R_FALSE;
	}
	
	if ((item = Find_Item(name)) != NULL
			&& (new_var=Create_Var(name, value)) != NULL)
	{
		if (item->str) 
		{
			free(item->str);
		}
		item->str = new_var;
		flag = R_TRUE;
	}

	return flag;
}


/*
 * purpose: determines if a string is a legal variable name
 * returns: 0 for no, 1 for yes
 */
/*  int Check_Name(char *str)
{
	char	*cp;
	for(cp = str; *cp; cp++ ){
		if ( (isdigit(*cp) && cp==str) || !(isalnum(*cp) || *cp=='_' ))
			return 0;
	}
	return ( cp != str );	// no empty strings, either 
}*/
/**
 * @Check_Name 
 * description: check name
 *            : valid----[0-9] in head, or contains ' ' ,'=',','  or contains a none-char
 *
 * @name: input string
 * @return : R_TRUE or R_FALSE
 */
int Check_Name(char *name)
{
	if (!name) 
	{
		return R_FALSE;
	}

	char * ck = name;

	if (isdigit(*ck))
	{
		return R_FALSE;
	}

	while (*ck)
	{
		if (*ck == ' ' || *ck == '=' || !(isalnum(*ck))) 
		{
			return R_FALSE;
		}
		ck++;	
	}
	return R_TRUE;
}



/**
 * @Env_To_Table :copy environ to g_Value
 *
 * @env[]: global enviroment (environ)
 *
 * @return: R_TRUE or R_FALSE
 */
int Env_To_Table(char *env[])
{
	char *new_str;

	int i = 0;
	while ((i < MAX_ARGS) && (NULL != env[i]))
	{
		new_str = malloc(1 + strlen(env[i]));
		if (!new_str)
		{	
			return R_FALSE;
		}
		strcpy(new_str, env[i]);
		g_Value[i].str = new_str;
		g_Value[i].global = 1;   //mark it to be global value by default
		i++;
	}	
	g_Value[i].str = NULL ;	
	g_Value[i].global = 0;

	return R_TRUE;
}

/*
 * build an array of pointers suig_Valuele for making a new environment
 * note, you need to free() this when done to avoid memory leaks
 */
char **Table_To_Env()
{
	int	i;			/* index	*/
	int j;			/* another index	*/
	int n;			/* counter	*/
	char **envtab;		/* array of pointers		*/

	/*
	 * first, count the number of global variables
	 */

	for( i = 0 ; (i < MAX_ARGS && g_Value[i].str != NULL) ; i++ )
		if (g_Value[i].global == 1)
			n++;

	/* then, allocate space for that many variables	*/
	envtab = (char **) malloc( (n+1) * sizeof(char *) );
	if ( envtab == NULL )
		return NULL;

	/* then, load the array with pointers		*/
	for(i = 0, j = 0 ; (i < MAX_ARGS && g_Value[i].str != NULL) ; i++ )
	{
		if ( g_Value[i].global == 1 )
		{
			envtab[j++] = g_Value[i].str;
		}
	}
	envtab[j] = NULL;
	return envtab;
}
