CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: snailfish

clean:
	rm -f snailfish snailfish.o sfnum.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

snailfish.o: snailfish.c sfnum.h

sfnum.o: sfnum.c sfnum.h

snailfish: snailfish.o sfnum.o
	$(CC) $(CFLAGS) $^ -o $@