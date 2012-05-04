/*
 ************************************************************************************
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ************************************************************************************
 * Filename     :  iwho.c
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Monday, April 30, 2012 04:49:57 HKT
 * Description  :  Print information about users who are currently logged in.
 *
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

/*#define SHOWHOST*/

void Show_Info(struct utmp *buf_fp);

int main(int argc, const char *argv[])
{
	struct utmp record;
	FILE   *utmp_fd;
	int    read_len = sizeof(record);

	/* UTMP_FILE is utmp.h */
	utmp_fd = fopen(UTMP_FILE, "r");

	while (fread(&record, read_len, 1, utmp_fd))
	{
		Show_Info(&record);	
	}

	fclose(utmp_fd);
	return 0;
}

void Show_Info(struct utmp *buf_fp)
{
	printf("%-8.8s", buf_fp->ut_id);
	printf(" ");
	printf("%-8.8s", buf_fp->ut_name);
	printf(" ");
	printf("%-8.8s", buf_fp->ut_line);
	printf(" ");
	printf("% 10ld", buf_fp->ut_time);
	printf(" ");
#ifdef SHOWHOST
	printf("(%s)", buf_fp->ut_host);
#endif
	printf("\n");
}
