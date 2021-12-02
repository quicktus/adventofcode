CC = gcc
DEBUG = -g -O0
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: scan

clean:
	rm -f scan scan.o

scan: scan.o
	$(CC) $(CFLAGS) scan.o -o scan

scan.o: scan.c
	$(CC) $(CFLAGS) $(DEBUG) scan.c -o scan.o -c