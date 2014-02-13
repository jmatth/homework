CC=gcc
CFLAGS=-Wall -g -Wno-unused-but-set-variable -lm

all: fork

fork: fork.c
	$(CC) $(CFLAGS) -o fork fork.c

clean:
	@rm fork

.PHONY: all clean
