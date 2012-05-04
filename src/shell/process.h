/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014  dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename      :  process.h
 * Version       :  1.0
 * Author        :  Jason Zhong
 * Created       :  Wednesday, April 25, 2012 09:44:37 HKT
 * Description   :  
 *
 * History       :
 * Revision		 :  none
 * Compiler      :  gcc
 ************************************************************************************
 */
#ifndef __PROCESS_H__
#define __PROCESS_H__

int Is_Control_Cmd(char *input);
int Do_Control_Cmd(char **);
int Ok_Execute();
void Syntax_Error(char *ckstr);
int Process_Buildin_Cmd(char **cmd);
int Process(char **input);
int IS_Buildin_Cmd(char **input);

#endif
