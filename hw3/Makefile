CC = gcc
CFLAGS = -Wall -g -pthread

all: part1 mypthread.o test

part1: part1.c
	$(CC) $(CFLAGS) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -o part1 part1.c

mypthread.o: mypthread.c mypthread.h
	$(CC) $(CFLAGS) -c mypthread.c

test: mypthread.o test.c
	$(CC) $(CFLAGS) -o test test.c mypthread.o

clean:
	@rm -rf part1 test
	@rm -rf *.o

.PHONY: clean
