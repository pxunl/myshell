/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  valuelib.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Wednesday, April 11, 2012 09:31:09 HKT
 * Description  :  system varlables
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
#include "valuelib.h"

extern struct Value g_Value[MAX_VALUE_NUM];

void Value_List()
{
	if (!g_Value) 
	{
		fatal("no any value!\n", NULL, 2);
	}
	
	int i = 0;
	char * var_out = g_Value;

	while ((NULL != var_out) && (i < MAX_VALUE_NUM))
	{
		if (var_out[i].global) 
		{
			printf("* %s\n", var_out[i]);
		}
		else 
		{
			printf(" %s\n", var_out[i]);
		}
		i++;
	}
	
}


