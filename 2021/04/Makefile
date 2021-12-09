CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: giantsquid

clean:
	rm -f giantsquid giantsquid.o

giantsquid: giantsquid.o
	$(CC) $(CFLAGS) giantsquid.o -o giantsquid

giantsquid.o: giantsquid.c
	$(CC) $(CFLAGS) $(DEBUG) giantsquid.c -o giantsquid.o -c