COMPILER = gcc

all: mysFunc

mysFunc: mysFunc.o mystery.o
	$(COMPILER) -o mysFunc mysFunc.o mystery.o

mystery.o: mystery.s
	$(COMPILER) -c mystery.s

mysFunc.o: mysFunc.c
	$(COMPILER) -c mysFunc.c

clean:
	rm -f mysFunc *.o
