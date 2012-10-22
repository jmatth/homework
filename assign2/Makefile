######################################
# CS211 sample makefile
######################################

COMPILER = gcc
# CCFLAGS  = -ansi -pedantic -Wall
CCFLAGS  = -Wall

all: dataConvertor

debug:
	make DEBUG=TRUE

dataConvertor: dataConvertor.o
	$(COMPILER) $(CCFLAGS) -o dataConvertor dataConvertor.o
dataConvertor.o: dataConvertor.c dataConvertor.h
	$(COMPILER) $(CCFLAGS) -c dataConvertor.c dataConvertor.h

ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g
endif

clean:
	rm -f dataConvertor
	rm -f *.o
	rm -f *.gch
	rm -f *.aux
	rm -f *.log
	rm -f *.pdf
