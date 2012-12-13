COMPILER = gcc
CCFLAGS = -Wall

all: cachesize cacheblock

debug:
	make DEBUG=TRUE

cachesize:
	$(COMPILER) -o cachesize cachesize.c

cacheblock:
	$(COMPILER) -o cacheblock cacheblock.c

ifeq ($(DEBUG),TRUE)
	CCFLAGS += -g
endif

clean:
	rm -f cachesize
	rm -f cacheblock
	rm -f *.o
