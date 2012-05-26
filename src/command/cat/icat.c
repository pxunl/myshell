/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  icat.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Saturday, May 06, 2012 11:47:57 HKT
 * Description  :  Concatenate FILE(s), or standard input, to standard output.
 * 				   -n(--number:number all output lines
 * History      :
 * Revision     :  try to support more options 
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
/*#include <signal.h>*/
/*#include <termios.h>*/


#define BUF_SIZE       128 /*The length of Line to be printed */  

#define program_name   "icat"

#define R_TRUE         0

#define R_FALSE        1

void usage(int status);
void do_cat(const char *file);
void do_cat_with_option(const char *file, const char *option);
int is_directory(const char *file);

int main(int argc, const char *argv[])
{
	/* only one file or one option is accpetable */
	if (argc != 2 && argc != 3)
	{
		usage(R_TRUE);
	}

	int i;

	/* no option */
	if (argc == 2) 
	{
		do_cat(argv[1]);
	}

	/*with option*/
	else if (argc == 3) 
	{
		for (i = 1; i < 3; i++) 
		{
			if (strpbrk(argv[i], "-n")) 
			{
				continue;
			}
			do_cat_with_option(argv[i], "-n");
		}
	}

	return 0;
}
void usage(int status)
{
	if (status != R_TRUE)
		fprintf (stderr, ("Try `%s --help' for more information.\n"),
				program_name);
	else
	{
		printf (("Usage: %s [OPTION]...\n"), program_name);
		printf(("\n%s\n%s\n"),"Concatenate FILE(s), or standard input, to standard output",
					"-n(--number:number all output lines");
   	}
	exit (status);
}

int is_directory(const char *file)
{
	struct stat stat_ck;
	if (stat(file, &stat_ck) == -1)
	{
		perror("stat");
	}
	if (S_ISDIR(stat_ck.st_mode))
	{
		return R_TRUE;
	}
	return R_FALSE;
}


/**
 * @do_cat_with_option: cat with option("-n"), add line number at
 * 						the beginning of all line
 * @file: file name
 * @option: option for cat
 */
void do_cat_with_option(const char *file, const char *option)
{
	if (file == NULL || option == NULL) 
	{
		return;
	}
	
	if (is_directory(file) == R_TRUE) 
	{
		printf("%s: %s: Is a directory\n", program_name, file);
		return;
	}
	
	/*char buf[BUF_SIZE];*/
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t n_read;
	long line_num = 0;

	if ((fp = fopen(file, "r")) == NULL)
	{
		exit(1);
	}

	//read one line each time
	while ((n_read = getline(&line, &len, fp)) != -1) 
	{
		printf("%ld %s", ++line_num, line); // add line number
	}

	if (line)
		free(line);

	fclose(fp);
}


/**
 * @do_cat: cat without any option, ignore if it is a directory
 *
 * @file: file name
 */
void do_cat(const char *file)
{
	if (is_directory(file) == R_TRUE) 
	{
		printf("%s: %s: Is a directory\n", program_name, file);
		return;
	}
	
	int read_size = 0;
	char buf[BUF_SIZE];
	int fd;

	if ((fd = open(file, O_RDONLY)) == -1) 
	{
		printf("can't read %s, please check it's permision\n", file);
		exit(1);
	}
	
	while ((read_size = read(fd, buf, BUF_SIZE)) != 0)
	{
		buf[read_size] = '\0';
		printf("%s", buf);
	}
	close(fd);
}
