CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: pdecoder

clean:
	rm -f pdecoder pdecoder.o

pdecoder: pdecoder.o
	$(CC) $(CFLAGS) pdecoder.o -o pdecoder

pdecoder.o: pdecoder.c
	$(CC) $(CFLAGS) pdecoder.c -o pdecoder.o -c