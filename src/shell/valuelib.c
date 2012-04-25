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
#include "shell.h"

typedef struct tag_Value 
{
	char *str;
	int  global;
} Value;

extern struct Value g_Value[MAX_VALU_NUM];

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
void Value_Export(char *name);
int Value_Store(char *name, char *value); 
int Value_Assign();
