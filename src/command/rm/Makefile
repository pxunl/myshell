#
 ####################################################################################
 # Copyright (c),  2011-2014 dd.pangxie@gmail.com
 ####################################################################################
 # Filename     :  mainWindow.c
 # Version      :  1.0
 # Author       :  Jason Zhong 
 # Created      :  Tue, March 6, 2012 12:19:02 HKT
 # Description  :  Makefile for compile gtk program
 #
 # History      :
 # Revision     :  none
 # Compiler     :  gcc
 #
 #####################################################################################
#

#Source file
SRC = irm.c

#Object file
OBJS = $(SRC:.c=.o)

#Output execution file
PROGRAM = irm 

#Compiler
CC = gcc


CFLAGS = -g -Wall

#Include 
INCLUDE = .


##########################################################
#dependency files (.o)
all:${PROGRAM}

${PROGRAM}:${OBJS}
	${CC} -o ${PROGRAM} ${OBJS} ${CFLAGS} -I $(INCLUDE)

${OBJS}:%.o:%.c 
	${CC} -c ${CFLAGS} $< -o $@


.PHONY:c
c:
	-rm *.o

r: clean all
