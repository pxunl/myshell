/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  icd.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Monday, May 07, 2012 04:57:25 HKT
 * Description  :  switch current directory
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
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#define HOME_DIR   "/home/pxunl/myshell/src/shell/"

void usage()
{
	printf("Usage: cd [OPTIONS] DIRECTORY\n icd --help");
}
	
int main(int argc, const char *argv[])
{
	if (argc == 1) 
	{
		chdir(HOME_DIR);
	}

	if (argc > 2) 
	{
		perror("only one directory is available\n");
		exit(1);
	}

	if (argc == 2)
	{
		chdir(argv[1]);
	}
	else
	{
		usage();
	}

	return 0;
}

