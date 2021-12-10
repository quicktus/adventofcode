CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: smokebasin

clean:
	rm -f smokebasin smokebasin.o

smokebasin: smokebasin.o
	$(CC) $(CFLAGS) smokebasin.o -o smokebasin

smokebasin.o: smokebasin.c
	$(CC) $(CFLAGS) $(DEBUG) smokebasin.c -o smokebasin.o -c
