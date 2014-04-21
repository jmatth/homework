CC = gcc
CFLAGS = -Wall -g -pthread

all: part1 mypthreads.o test

part1: part1.c
	$(CC) $(CFLAGS) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -o part1 part1.c

mypthreads.o: mypthreads.c mypthreads.h
	$(CC) $(CFLAGS) -c mypthreads.c

test: mypthreads.o test.c
	$(CC) $(CFLAGS) -o test test.c mypthreads.o

clean:
	@rm -rf part1
	@rm -rf *.o

.PHONY: clean
