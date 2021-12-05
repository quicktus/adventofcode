CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: dive

clean:
	rm -f dive dive.o

dive: dive.o
	$(CC) $(CFLAGS) dive.o -o dive

dive.o: dive.c
	$(CC) $(CFLAGS) $(DEBUG) dive.c -o dive.o -c