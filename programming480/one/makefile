# A Makefile for Program 1, CS570

TARGET = mulproc
OLD = alphabetcount.o specialcount.o

all: ${TARGET}

CC = gcc
CFLAGS = -g -std=c99


${TARGET}:	${TARGET}.o ${OLD}
		${CC} -o ${TARGET} ${TARGET}.o ${OLD}


clean:
		rm -f ${TARGET} ${TARGET}.o alphabetcount specialcount
