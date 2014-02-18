CC=gcc
CFLAGS=-Wall -g

all: fork pipe

fork: fork.c
	$(CC) $(CFLAGS) -o fork fork.c

pipe: pipe.c
	$(CC) $(CFLAGS) -o pipe pipe.c

clean:
	@rm fork
	@rm pipe

.PHONY: all clean
