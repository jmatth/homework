CC=gcc
CFLAGS=-Wall -g

all: cacheLine fork pipe dummy

cacheLine: cacheLine.c
	$(CC) $(CFLAGS) -o cacheLine cacheLine.c

fork: fork.c
	$(CC) $(CFLAGS) -o fork fork.c

pipe: pipe.c
	$(CC) $(CFLAGS) -o pipe pipe.c

dummy: dummy.c
	$(CC) $(CFLAGS) -o dummy dummy.c

clean:
	@rm -f fork
	@rm -f pipe
	@rm -f dummy
	@rm -f cacheLine

.PHONY: all clean
