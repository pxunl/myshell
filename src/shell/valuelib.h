/*
 ************************************************************************************
 *
 * Copyright (c),  2011-2014  dd.pangxie@gmail.com
 *
 ************************************************************************************
 * Filename      :  valuelib.h
 * Version       :  1.0
 * Author        :  Jason Zhong
 * Created       :  Sunday, April 15, 2012 03:51:33 HKT
 * Description   :  functions for process value that build in.
 *
 * History       :
 * Revision		 :  none
 * Compiler      :  gcc
 ************************************************************************************
 */
#ifndef __VALUE_LIB_H
#define __VALUE_LIB_H



#define MAX_VALUE_NUM   100

struct tag_ValueLib 
{
	char *value;
	int global;
}Value;

void Value_List();

int Value_Add();

int Value_Del();

int Check_Name(char *ck);

#endif

