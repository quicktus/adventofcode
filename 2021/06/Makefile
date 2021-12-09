CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: lanternfish

clean:
	rm -f lanternfish lanternfish.o

lanternfish: lanternfish.o
	$(CC) $(CFLAGS) lanternfish.o -o lanternfish

lanternfish.o: lanternfish.c
	$(CC) $(CFLAGS) $(DEBUG) lanternfish.c -o lanternfish.o -c
