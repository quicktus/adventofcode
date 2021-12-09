CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: ssweep

clean:
	rm -f ssweep ssweep.o

ssweep: ssweep.o
	$(CC) $(CFLAGS) ssweep.o -o ssweep

ssweep.o: ssweep.c
	$(CC) $(CFLAGS) $(DEBUG) ssweep.c -o ssweep.o -c