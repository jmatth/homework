CC = gcc
CFLAGS = -Wall -g

all: part1 mypthread.o mypthread-sort

part1: part1.c
	$(CC) $(CFLAGS) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -pthread -o part1 part1.c

mypthread-sort: mypthread-sort.c mypthread.o
	$(CC) $(CFLAGS) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -o mypthread-sort mypthread-sort.c mypthread.o

mypthread.o: mypthread.c mypthread.h
	$(CC) $(CFLAGS) -c mypthread.c

clean:
	@rm -rf part1 mypthread-sort
	@rm -rf *.o

.PHONY: clean
