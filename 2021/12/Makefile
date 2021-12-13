CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: ppathing

clean:
	rm -f ppathing ppathing.o

ppathing: ppathing.o
	$(CC) $(CFLAGS) ppathing.o -o ppathing

ppathing.o: ppathing.c
	$(CC) $(CFLAGS) $(DEBUG) ppathing.c -o ppathing.o -c
