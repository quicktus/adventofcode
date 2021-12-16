CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: chiton

clean:
	rm -f chiton chiton.o

chiton: chiton.o
	$(CC) $(CFLAGS) chiton.o -o chiton

chiton.o: chiton.c
	$(CC) $(CFLAGS) chiton.c -o chiton.o -c