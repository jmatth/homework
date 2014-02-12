######################################
# CS211 sample makefile
######################################

COMPILER = gcc
# CCFLAGS  = -ansi -pedantic -Wall
CCFLAGS  = -Wall

all: prefixStat

debug:
	make DEBUG=TRUE

prefixStat: prefixStat.o
	$(COMPILER) $(CCFLAGS) -o prefixStat prefixStat.o
prefixStat.o: prefixStat.c prefixStat.h
	$(COMPILER) $(CCFLAGS) -c prefixStat.c prefixStat.h

ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g
endif

clean:
	rm -f prefixStat
	rm -f *.o
	rm -f *.gch
	rm -f *.aux
	rm -f *.log
	rm -f *.pdf
