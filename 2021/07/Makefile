CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: ttowhales

clean:
	rm -f ttowhales ttowhales.o

ttowhales: ttowhales.o
	$(CC) $(CFLAGS) ttowhales.o -o ttowhales

ttowhales.o: ttowhales.c
	$(CC) $(CFLAGS) $(DEBUG) ttowhales.c -o ttowhales.o -c
