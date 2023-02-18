CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm

all: shell

shell: shell.c
	 ${CC} -o shell shell.c

clean:
	rm -f *.o *~ 