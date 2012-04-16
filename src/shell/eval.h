/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  eval.h
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Saturday, March 03, 2012 9:13:00 HK
 * Description  :  
 	
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
#ifndef __EVAL_H__
#define __EVAL_H__ 

#define BUFSIZE 128

char * create_str(char *begin, int len);

char **spiltline(char *cmdline);

char * read_command(FILE *fp);

void fatal(char *str_err, char *str_error, int exit_code);

void free_list(char **list);

#endif
