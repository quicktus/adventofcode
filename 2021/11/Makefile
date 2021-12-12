CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: doctopus

clean:
	rm -f doctopus doctopus.o

doctopus: doctopus.o
	$(CC) $(CFLAGS) doctopus.o -o doctopus

doctopus.o: doctopus.c
	$(CC) $(CFLAGS) $(DEBUG) doctopus.c -o doctopus.o -c
