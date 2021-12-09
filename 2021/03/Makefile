CC = gcc
DEBUG = -g -O0
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: binarydiagnostic

clean:
	rm -f bindiagnostic bindiagnostic.o

bindiagnostic: bindiagnostic.o
	$(CC) $(CFLAGS) bindiagnostic.o -o bindiagnostic

bindiagnostic.o: bindiagnostic.c
	$(CC) $(CFLAGS) $(DEBUG) bindiagnostic.c -o bindiagnostic.o -c