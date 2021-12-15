CC = gcc
DEBUG = -g
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 

.PHONY: all clean

all: xpoly

clean:
	rm -f xpoly xpoly.o polyList.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

xpoly.o: xpoly.c polyList.h

polyList.o: polyList.c polyList.h

xpoly: xpoly.o polyList.o
	$(CC) $(CFLAGS) $^ -o $@