
/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014 dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename     :  shell.h 
 * Version      :  1.0
 * Author       :  Jason Zhong 
 * Created      :  Saturday, April 11, 2012 12:13:00 HKT
 * Description  :  some inportant functions for shell (main)
 *
 * History      :
 * Revision     :  none
 * Compiler     :  gcc
 *
 *************************************************************************************
 */
 
#ifndef __SHELL_H__
#define __SHELL_H__

#define MAX_ARGS   50
#define ARG_LEN    100


int Process(char **input);

int execute(char *argv[]);
void fatal(char *str_err, char *str_error, int exit_code);

#endif
