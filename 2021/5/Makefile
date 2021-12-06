CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: htventure

clean:
	rm -f htventure htventure.o

htventure: htventure.o
	$(CC) $(CFLAGS) htventure.o -o htventure

htventure.o: htventure.c
	$(CC) $(CFLAGS) $(DEBUG) htventure.c -o htventure.o -c
