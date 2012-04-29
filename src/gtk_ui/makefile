#
#Author:PXunL
#dd.pangxie@gmil.com
#2012-3-6
#Makefile for compile gtk program

#Source file
SRC = mainWindow.c shell.c eval.c execute.c process.c valuelib.c

#Object file
OBJS = $(SRC:.c=.o)

#Output execution file
PROGRAM = shell

#Compiler
CC = gcc


CFLAGS = -g -Wall

#Include 
INCLUDE = ../shell

#Libs
LIBS= --libs gtk+-2.0

CFLAGS = `pkg-config --cflags ${LIBS}` -g -Wall


##########################################################
#dependency files (.o)
all:${PROGRAM}

${PROGRAM}:${OBJS}
	${CC} -o ${PROGRAM} ${OBJS} ${CFLAGS} -I $(INCLUDE)

${OBJS}:%.o:%.c 
	${CC} -c ${CFLAGS} $< -o $@



.PHONY:clean
clean:
	-rm *.o

r: clean all
