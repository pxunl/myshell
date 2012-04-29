/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014  dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename      :  valuelib.h
 * Version       :  1.0
 * Author        :  Jason Zhong
 * Created       :  Monday, April 16, 2012 03:54:00 HKT
 * Description   :  
 *
 * History       :
 * Revision		 :  none
 * Compiler      :  gcc
 ************************************************************************************
 */
#ifndef __VALUELIB_H__
#define __VALUELIB_H__

void Value_List();
int Value_Export(char *name);
int Value_Store(char *var);
int Check_Name(char *str);
int Env_To_Table(char *env[]);
char **Table_To_Env();

#endif
