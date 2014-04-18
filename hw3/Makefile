CC = gcc
CFLAGS = -Wall -g -pthread

all: part1

part1: part1.c
	$(CC) $(CFLAGS) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -o part1 part1.c

clean:
	@rm -rf part1

.PHONY: clean
