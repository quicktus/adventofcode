CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: bscanner

clean:
	rm -f bscanner bscanner.o

bscanner: bscanner.o
	$(CC) $(CFLAGS) bscanner.o -o bscanner

bscanner.o: bscanner.c
	$(CC) $(CFLAGS) bscanner.c -o bscanner.o -c