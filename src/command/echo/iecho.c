/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  iecho.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Monday, May 07, 2012 03:15:27 HKT
 * Description  :  Echo the STRING(s) to standard output.
 * 				   -r sort and output
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


void do_echo(const char *output, int flag)
{
	if (1 == flag) 
	{
		printf("%s\n", output);
	}
	else 
	{
		printf("%s  ", output);
	}
}

int main(int argc, const char *argv[])
{
	if (argc < 2) 
	{
		printf("Nothing to output\n");
		return 0;
	}

	int  i;
	int flag_r = 0;
	for (i = 1; i < argc; i++) 
	{
		if (strpbrk(argv[i], "-r") != NULL) 
		{
			flag_r = 1;
		}
	}

	for (i = 1; i < argc; i++) 
	{
		if (1 == flag_r) 
		{
			do_echo(argv[i], 1);
		}
		else 
		{
			do_echo(argv[i], 0);
		}
	}
	
	return 0;
}
