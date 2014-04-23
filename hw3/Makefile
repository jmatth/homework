CC = gcc
CFLAGS = -Wall -g

all: part1 mypthread.o test myretardsort

part1: part1.c
	$(CC) $(CFLAGS) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -pthread -o part1 part1.c

myretardsort: myretardsort.c mypthread.o
	$(CC) $(CFLAGS) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -o myretardsort myretardsort.c mypthread.o

mypthread.o: mypthread.c mypthread.h
	$(CC) $(CFLAGS) -c mypthread.c

test: mypthread.o test.c
	$(CC) $(CFLAGS) -o test test.c mypthread.o

clean:
	@rm -rf part1 test myretardsort
	@rm -rf *.o

.PHONY: clean
