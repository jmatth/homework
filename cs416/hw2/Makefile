CC = gcc
CFLAGS = -Wall -g

all: part1

part1: part1.c
	$(CC) $(CFLAGS) -m32 -Wno-div-by-zero -Wno-unused-variable -o part1 part1.c

clean:
	@rm -rf part1

.PHONY: clean
