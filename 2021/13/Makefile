CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: torigami

clean:
	rm -f torigami torigami.o

torigami: torigami.o
	$(CC) $(CFLAGS) torigami.o -o torigami

torigami.o: torigami.c
	$(CC) $(CFLAGS) $(DEBUG) torigami.c -o torigami.o -c