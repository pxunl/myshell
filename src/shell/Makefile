#
 ####################################################################################
 #
 # Copyright (c),  2011-2014 dd.pangxie@gmail.com
 #
 ####################################################################################
 # Filename     :  Makefile
 # Version      :  1.0
 # Author       :  Jason Zhong 
 # Created      :  Saturday, March 07, 2012 12:13:00 HKT
 # Description  :  Makefile for project myshell
 #
 # History      :
 # Revision     :  none
 # Compiler     :  gcc
 #
 #####################################################################################
 #

#Source file
SRC =  shell.c eval.c execute.c process.c valuelib.c

#Object file
OBJS = $(SRC:.c=.o)

#Output execution file
PROGRAM = shell

#Compiler
CC = gcc


CFLAGS = -g -Wall


##########################################################
#dependency files (.o)
all:${PROGRAM}

${PROGRAM}:${OBJS}
	${CC} -o ${PROGRAM} ${OBJS} ${CFLAGS}

${OBJS}:%.o:%.c
	${CC} -c ${CFLAGS} $< -o $@



.PHONY:clean
clean:
	-rm *.o

r: clean all
