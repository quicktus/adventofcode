CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: sesesearch

clean:
	rm -f sesesearch sesesearch.o

sesesearch: sesesearch.o
	$(CC) $(CFLAGS) sesesearch.o -o sesesearch

sesesearch.o: sesesearch.c
	$(CC) $(CFLAGS) $(DEBUG) sesesearch.c -o sesesearch.o -c
