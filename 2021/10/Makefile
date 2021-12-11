CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: synscoring

clean:
	rm -f synscoring synscoring.o

synscoring: synscoring.o
	$(CC) $(CFLAGS) synscoring.o -o synscoring

synscoring.o: synscoring.c
	$(CC) $(CFLAGS) $(DEBUG) synscoring.c -o synscoring.o -c
