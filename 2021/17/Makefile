CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: tshot

clean:
	rm -f tshot tshot.o

tshot: tshot.o
	$(CC) $(CFLAGS) tshot.o -o tshot

tshot.o: tshot.c
	$(CC) $(CFLAGS) tshot.c -o tshot.o -c